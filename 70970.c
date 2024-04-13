monitor_child (int event_fd, pid_t child_pid, int setup_finished_fd)
{
  int res;
  uint64_t val;
  ssize_t s;
  int signal_fd;
  sigset_t mask;
  struct pollfd fds[2];
  int num_fds;
  struct signalfd_siginfo fdsi;
  int dont_close[] = {-1, -1, -1, -1};
  int j = 0;
  int exitc;
  pid_t died_pid;
  int died_status;

  /* Close all extra fds in the monitoring process.
     Any passed in fds have been passed on to the child anyway. */
  if (event_fd != -1)
    dont_close[j++] = event_fd;
  if (opt_json_status_fd != -1)
    dont_close[j++] = opt_json_status_fd;
  if (setup_finished_fd != -1)
    dont_close[j++] = setup_finished_fd;
  assert (j < sizeof(dont_close)/sizeof(*dont_close));
  fdwalk (proc_fd, close_extra_fds, dont_close);

  sigemptyset (&mask);
  sigaddset (&mask, SIGCHLD);

  signal_fd = signalfd (-1, &mask, SFD_CLOEXEC | SFD_NONBLOCK);
  if (signal_fd == -1)
    die_with_error ("Can't create signalfd");

  num_fds = 1;
  fds[0].fd = signal_fd;
  fds[0].events = POLLIN;
  if (event_fd != -1)
    {
      fds[1].fd = event_fd;
      fds[1].events = POLLIN;
      num_fds++;
    }

  while (1)
    {
      fds[0].revents = fds[1].revents = 0;
      res = poll (fds, num_fds, -1);
      if (res == -1 && errno != EINTR)
        die_with_error ("poll");

      /* Always read from the eventfd first, if pid 2 died then pid 1 often
       * dies too, and we could race, reporting that first and we'd lose
       * the real exit status. */
      if (event_fd != -1)
        {
          s = read (event_fd, &val, 8);
          if (s == -1 && errno != EINTR && errno != EAGAIN)
            die_with_error ("read eventfd");
          else if (s == 8)
            {
              exitc = (int) val - 1;
              report_child_exit_status (exitc, setup_finished_fd);
              return exitc;
            }
        }

      /* We need to read the signal_fd, or it will keep polling as read,
       * however we ignore the details as we get them from waitpid
       * below anyway */
      s = read (signal_fd, &fdsi, sizeof (struct signalfd_siginfo));
      if (s == -1 && errno != EINTR && errno != EAGAIN)
        die_with_error ("read signalfd");

      /* We may actually get several sigchld compressed into one
         SIGCHLD, so we have to handle all of them. */
      while ((died_pid = waitpid (-1, &died_status, WNOHANG)) > 0)
        {
          /* We may be getting sigchild from other children too. For instance if
             someone created a child process, and then exec:ed bubblewrap. Ignore them */
          if (died_pid == child_pid)
            {
              exitc = propagate_exit_status (died_status);
              report_child_exit_status (exitc, setup_finished_fd);
              return exitc;
            }
        }
    }

  die ("Should not be reached");

  return 0;
}
