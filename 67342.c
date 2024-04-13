mrb_io_close_write(mrb_state *mrb, mrb_value self)
{
  struct mrb_io *fptr;
  fptr = io_get_open_fptr(mrb, self);
  if (close((int)fptr->fd2) == -1) {
    mrb_sys_fail(mrb, "close");
  }
  return mrb_nil_value();
}
