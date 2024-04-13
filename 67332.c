io_get_open_fptr(mrb_state *mrb, mrb_value self)
{
  struct mrb_io *fptr;

  fptr = (struct mrb_io *)mrb_get_datatype(mrb, self, &mrb_io_type);
  if (fptr->fd < 0) {
    mrb_raise(mrb, E_IO_ERROR, "closed stream.");
  }
  return fptr;
}
