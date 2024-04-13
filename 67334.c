mrb_cloexec_open(mrb_state *mrb, const char *pathname, mrb_int flags, mrb_int mode)
{
  mrb_value emsg;
  int fd, retry = FALSE;
  char* fname = mrb_locale_from_utf8(pathname, -1);

#ifdef O_CLOEXEC
  /* O_CLOEXEC is available since Linux 2.6.23.  Linux 2.6.18 silently ignore it. */
  flags |= O_CLOEXEC;
#elif defined O_NOINHERIT
  flags |= O_NOINHERIT;
#endif
reopen:
  fd = open(fname, (int)flags, (fmode_t)mode);
  if (fd == -1) {
    if (!retry) {
      switch (errno) {
        case ENFILE:
        case EMFILE:
        mrb_garbage_collect(mrb);
        retry = TRUE;
        goto reopen;
      }
    }

    emsg = mrb_format(mrb, "open %S", mrb_str_new_cstr(mrb, pathname));
    mrb_str_modify(mrb, mrb_str_ptr(emsg));
    mrb_sys_fail(mrb, RSTRING_PTR(emsg));
  }
  mrb_utf8_free(fname);

  if (fd <= 2) {
    mrb_fd_cloexec(mrb, fd);
  }
  return fd;
}
