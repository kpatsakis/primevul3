fiber_current(mrb_state *mrb, mrb_value self)
{
  if (!mrb->c->fib) {
    struct RFiber *f = (struct RFiber*)mrb_obj_alloc(mrb, MRB_TT_FIBER, mrb_class_ptr(self));

    f->cxt = mrb->c;
    mrb->c->fib = f;
  }
  return mrb_obj_value(mrb->c->fib);
}
