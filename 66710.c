fiber_switch_context(mrb_state *mrb, struct mrb_context *c)
{
  c->status = MRB_FIBER_RUNNING;
  mrb->c = c;
}
