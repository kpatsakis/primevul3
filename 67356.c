mrb_io_set_sync(mrb_state *mrb, mrb_value self)
{
  struct mrb_io *fptr;
  mrb_bool b;

  fptr = io_get_open_fptr(mrb, self);
  mrb_get_args(mrb, "b", &b);
  fptr->sync = b;
  return mrb_bool_value(b);
}
