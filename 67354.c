mrb_io_s_sysclose(mrb_state *mrb, mrb_value klass)
{
  mrb_int fd;
  mrb_get_args(mrb, "i", &fd);
  if (close((int)fd) == -1) {
    mrb_sys_fail(mrb, "close");
  }
  return mrb_fixnum_value(0);
}
