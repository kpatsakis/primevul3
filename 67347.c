mrb_io_isatty(mrb_state *mrb, mrb_value self)
{
  struct mrb_io *fptr;

  fptr = io_get_open_fptr(mrb, self);
  if (isatty(fptr->fd) == 0)
    return mrb_false_value();
  return mrb_true_value();
}
