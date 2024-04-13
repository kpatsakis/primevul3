mrb_io_close(mrb_state *mrb, mrb_value self)
{
  struct mrb_io *fptr;
  fptr = io_get_open_fptr(mrb, self);
  fptr_finalize(mrb, fptr, FALSE);
  return mrb_nil_value();
}
