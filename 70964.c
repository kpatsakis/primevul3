do_init (int event_fd, pid_t initial_pid, struct sock_fprog *seccomp_prog)
{
  int initial_exit_status = 1;
  LockFile *lock;

  for (lock = lock_files; lock != NULL; lock = lock->next)
    {
      int fd = open (lock->path, O_RDONLY | O_CLOEXEC);
      if (fd == -1)
        die_with_error ("Unable to open lock file %s", lock->path);

      struct flock l = {
        .l_type = F_RDLCK,
        .l_whence = SEEK_SET,
        .l_start = 0,
        .l_len = 0
      };

      if (fcntl (fd, F_SETLK, &l) < 0)
        die_with_error ("Unable to lock file %s", lock->path);

      /* Keep fd open to hang on to lock */
      lock->fd = fd;
    }

  /* Optionally bind our lifecycle to that of the caller */
  handle_die_with_parent ();

  if (seccomp_prog != NULL &&
      prctl (PR_SET_SECCOMP, SECCOMP_MODE_FILTER, seccomp_prog) != 0)
    die_with_error ("prctl(PR_SET_SECCOMP)");

  while (TRUE)
    {
      pid_t child;
      int status;

      child = wait (&status);
      if (child == initial_pid && event_fd != -1)
        {
          uint64_t val;
          int res UNUSED;

          initial_exit_status = propagate_exit_status (status);

          val = initial_exit_status + 1;
          res = write (event_fd, &val, 8);
          /* Ignore res, if e.g. the parent died and closed event_fd
             we don't want to error out here */
        }

      if (child == -1 && errno != EINTR)
        {
          if (errno != ECHILD)
            die_with_error ("init wait()");
          break;
        }
    }

  /* Close FDs. */
  for (lock = lock_files; lock != NULL; lock = lock->next)
    {
      if (lock->fd >= 0)
        {
          close (lock->fd);
          lock->fd = -1;
        }
    }

  return initial_exit_status;
}
