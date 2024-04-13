fiber_resume(mrb_state *mrb, mrb_value self)
{
  mrb_value *a;
  mrb_int len;
  mrb_bool vmexec = FALSE;

  mrb_get_args(mrb, "*!", &a, &len);
  if (mrb->c->ci->acc < 0) {
    vmexec = TRUE;
  }
  return fiber_switch(mrb, self, len, a, TRUE, vmexec);
}
