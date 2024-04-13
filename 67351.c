mrb_io_s_popen(mrb_state *mrb, mrb_value klass)
{
  mrb_value cmd, io, result;
  mrb_value mode = mrb_str_new_cstr(mrb, "r");
  mrb_value opt  = mrb_hash_new(mrb);

  struct mrb_io *fptr;
  const char *pname;
  int pid, flags, fd, write_fd = -1;
  int pr[2] = { -1, -1 };
  int pw[2] = { -1, -1 };
  int doexec;
  int saved_errno;
  int opt_in, opt_out, opt_err;

  mrb_get_args(mrb, "S|SH", &cmd, &mode, &opt);
  io = mrb_obj_value(mrb_data_object_alloc(mrb, mrb_class_ptr(klass), NULL, &mrb_io_type));

  pname = mrb_string_value_cstr(mrb, &cmd);
  flags = mrb_io_modestr_to_flags(mrb, mrb_string_value_cstr(mrb, &mode));

  doexec = (strcmp("-", pname) != 0);
  opt_in = option_to_fd(mrb, opt, "in");
  opt_out = option_to_fd(mrb, opt, "out");
  opt_err = option_to_fd(mrb, opt, "err");

  if (flags & FMODE_READABLE) {
    if (pipe(pr) == -1) {
      mrb_sys_fail(mrb, "pipe");
    }
    mrb_fd_cloexec(mrb, pr[0]);
    mrb_fd_cloexec(mrb, pr[1]);
  }

  if (flags & FMODE_WRITABLE) {
    if (pipe(pw) == -1) {
      if (pr[0] != -1) close(pr[0]);
      if (pr[1] != -1) close(pr[1]);
      mrb_sys_fail(mrb, "pipe");
    }
    mrb_fd_cloexec(mrb, pw[0]);
    mrb_fd_cloexec(mrb, pw[1]);
  }

  if (!doexec) {
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);
  }

  result = mrb_nil_value();
  switch (pid = fork()) {
    case 0: /* child */
      if (opt_in != -1) {
        dup2(opt_in, 0);
      }
      if (opt_out != -1) {
        dup2(opt_out, 1);
      }
      if (opt_err != -1) {
        dup2(opt_err, 2);
      }
      if (flags & FMODE_READABLE) {
        close(pr[0]);
        if (pr[1] != 1) {
          dup2(pr[1], 1);
          close(pr[1]);
        }
      }
      if (flags & FMODE_WRITABLE) {
        close(pw[1]);
        if (pw[0] != 0) {
          dup2(pw[0], 0);
          close(pw[0]);
        }
      }
      if (doexec) {
        for (fd = 3; fd < NOFILE; fd++) {
          close(fd);
        }
        mrb_proc_exec(pname);
        mrb_raisef(mrb, E_IO_ERROR, "command not found: %S", cmd);
        _exit(127);
      }
      result = mrb_nil_value();
      break;

    default: /* parent */
      if ((flags & FMODE_READABLE) && (flags & FMODE_WRITABLE)) {
        close(pr[1]);
        fd = pr[0];
        close(pw[0]);
        write_fd = pw[1];
      } else if (flags & FMODE_READABLE) {
        close(pr[1]);
        fd = pr[0];
      } else {
        close(pw[0]);
        fd = pw[1];
      }

      mrb_iv_set(mrb, io, mrb_intern_cstr(mrb, "@buf"), mrb_str_new_cstr(mrb, ""));

      fptr = mrb_io_alloc(mrb);
      fptr->fd = fd;
      fptr->fd2 = write_fd;
      fptr->pid = pid;
      fptr->readable = ((flags & FMODE_READABLE) != 0);
      fptr->writable = ((flags & FMODE_WRITABLE) != 0);
      fptr->sync = 0;

      DATA_TYPE(io) = &mrb_io_type;
      DATA_PTR(io)  = fptr;
      result = io;
      break;

    case -1: /* error */
      saved_errno = errno;
      if (flags & FMODE_READABLE) {
        close(pr[0]);
        close(pr[1]);
      }
      if (flags & FMODE_WRITABLE) {
        close(pw[0]);
        close(pw[1]);
      }
      errno = saved_errno;
      mrb_sys_fail(mrb, "pipe_open failed.");
      break;
  }
  return result;
}
