mrb_io_check_readable(mrb_state *mrb, mrb_value self)
{
  struct mrb_io *fptr = io_get_open_fptr(mrb, self);
  if (! fptr->readable) {
    mrb_raise(mrb, E_IO_ERROR, "not opened for reading");
  }
  return mrb_nil_value();
}
