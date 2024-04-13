mrb_io_sync(mrb_state *mrb, mrb_value self)
{
  struct mrb_io *fptr;
  fptr = io_get_open_fptr(mrb, self);
  return mrb_bool_value(fptr->sync);
}
