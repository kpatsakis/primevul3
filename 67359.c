mrb_io_syswrite(mrb_state *mrb, mrb_value io)
{
  struct mrb_io *fptr;
  mrb_value str, buf;
  int fd, length;

  fptr = (struct mrb_io *)mrb_get_datatype(mrb, io, &mrb_io_type);
  if (! fptr->writable) {
    mrb_raise(mrb, E_IO_ERROR, "not opened for writing");
  }

  mrb_get_args(mrb, "S", &str);
  if (mrb_type(str) != MRB_TT_STRING) {
    buf = mrb_funcall(mrb, str, "to_s", 0);
  } else {
    buf = str;
  }

  if (fptr->fd2 == -1) {
    fd = fptr->fd;
  } else {
    fd = fptr->fd2;
  }
  length = write(fd, RSTRING_PTR(buf), (fsize_t)RSTRING_LEN(buf));
  if (length == -1) {
    mrb_sys_fail(mrb, 0);
  }

  return mrb_fixnum_value(length);
}
