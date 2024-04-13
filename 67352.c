mrb_io_s_popen(mrb_state *mrb, mrb_value klass)
{
  mrb_value cmd, io;
  mrb_value mode = mrb_str_new_cstr(mrb, "r");
  mrb_value opt  = mrb_hash_new(mrb);

  struct mrb_io *fptr;
  const char *pname;
  int pid = 0, flags;
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  SECURITY_ATTRIBUTES saAttr;

  HANDLE ifd[2];
  HANDLE ofd[2];

  int doexec;
  int opt_in, opt_out, opt_err;

  ifd[0] = INVALID_HANDLE_VALUE;
  ifd[1] = INVALID_HANDLE_VALUE;
  ofd[0] = INVALID_HANDLE_VALUE;
  ofd[1] = INVALID_HANDLE_VALUE;

  mrb_get_args(mrb, "S|SH", &cmd, &mode, &opt);
  io = mrb_obj_value(mrb_data_object_alloc(mrb, mrb_class_ptr(klass), NULL, &mrb_io_type));

  pname = mrb_string_value_cstr(mrb, &cmd);
  flags = mrb_io_modestr_to_flags(mrb, mrb_string_value_cstr(mrb, &mode));

  doexec = (strcmp("-", pname) != 0);
  opt_in = option_to_fd(mrb, opt, "in");
  opt_out = option_to_fd(mrb, opt, "out");
  opt_err = option_to_fd(mrb, opt, "err");

  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;
  saAttr.lpSecurityDescriptor = NULL;

  if (flags & FMODE_READABLE) {
    if (!CreatePipe(&ofd[0], &ofd[1], &saAttr, 0)
        || !SetHandleInformation(ofd[0], HANDLE_FLAG_INHERIT, 0)) {
      mrb_sys_fail(mrb, "pipe");
    }
  }

  if (flags & FMODE_WRITABLE) {
    if (!CreatePipe(&ifd[0], &ifd[1], &saAttr, 0)
        || !SetHandleInformation(ifd[1], HANDLE_FLAG_INHERIT, 0)) {
      mrb_sys_fail(mrb, "pipe");
    }
  }

  if (doexec) {
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags |= STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    si.dwFlags |= STARTF_USESTDHANDLES;
    if (flags & FMODE_READABLE) {
      si.hStdOutput = ofd[1];
      si.hStdError = ofd[1];
    }
    if (flags & FMODE_WRITABLE) {
      si.hStdInput = ifd[0];
    }
    if (!CreateProcess(
        NULL, (char*)pname, NULL, NULL,
        TRUE, CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi)) {
      CloseHandle(ifd[0]);
      CloseHandle(ifd[1]);
      CloseHandle(ofd[0]);
      CloseHandle(ofd[1]);
      mrb_raisef(mrb, E_IO_ERROR, "command not found: %S", cmd);
    }
    CloseHandle(pi.hThread);
    CloseHandle(ifd[0]);
    CloseHandle(ofd[1]);
    pid = pi.dwProcessId;
  }

  mrb_iv_set(mrb, io, mrb_intern_cstr(mrb, "@buf"), mrb_str_new_cstr(mrb, ""));

  fptr = mrb_io_alloc(mrb);
  fptr->fd = _open_osfhandle((intptr_t)ofd[0], 0);
  fptr->fd2 = _open_osfhandle((intptr_t)ifd[1], 0);
  fptr->pid = pid;
  fptr->readable = ((flags & FMODE_READABLE) != 0);
  fptr->writable = ((flags & FMODE_WRITABLE) != 0);
  fptr->sync = 0;

  DATA_TYPE(io) = &mrb_io_type;
  DATA_PTR(io)  = fptr;
  return io;
}
