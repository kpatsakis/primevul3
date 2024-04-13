mrb_io_closed(mrb_state *mrb, mrb_value io)
{
  struct mrb_io *fptr;
  fptr = (struct mrb_io *)mrb_get_datatype(mrb, io, &mrb_io_type);
  if (fptr->fd >= 0) {
    return mrb_false_value();
  }

  return mrb_true_value();
}
