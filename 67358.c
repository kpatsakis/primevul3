mrb_io_sysseek(mrb_state *mrb, mrb_value io)
{
  struct mrb_io *fptr;
  off_t pos;
  mrb_int offset, whence = -1;

  mrb_get_args(mrb, "i|i", &offset, &whence);
  if (whence < 0) {
    whence = 0;
  }

  fptr = (struct mrb_io *)mrb_get_datatype(mrb, io, &mrb_io_type);
  pos = lseek(fptr->fd, (off_t)offset, (int)whence);
  if (pos == -1) {
    mrb_sys_fail(mrb, "sysseek");
  }
  if (pos > MRB_INT_MAX) {
#ifndef MRB_WITHOUT_FLOAT
    return mrb_float_value(mrb, (mrb_float)pos);
#else
    mrb_raise(mrb, E_IO_ERROR, "sysseek reached too far for MRB_WITHOUT_FLOAT");
#endif
  } else {
    return mrb_fixnum_value(pos);
  }
}
