stack_extend(mrb_state *mrb, int room)
{
  if (mrb->c->stack + room >= mrb->c->stend) {
    stack_extend_alloc(mrb, room);
  }
}
