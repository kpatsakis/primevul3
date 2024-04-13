mrb_mruby_fiber_gem_init(mrb_state* mrb)
{
  struct RClass *c;

  c = mrb_define_class(mrb, "Fiber", mrb->object_class);
  MRB_SET_INSTANCE_TT(c, MRB_TT_FIBER);

  mrb_define_method(mrb, c, "initialize", fiber_init,    MRB_ARGS_NONE());
  mrb_define_method(mrb, c, "resume",     fiber_resume,  MRB_ARGS_ANY());
  mrb_define_method(mrb, c, "transfer",   fiber_transfer, MRB_ARGS_ANY());
  mrb_define_method(mrb, c, "alive?",     fiber_alive_p, MRB_ARGS_NONE());
  mrb_define_method(mrb, c, "==",         fiber_eq,      MRB_ARGS_REQ(1));

  mrb_define_class_method(mrb, c, "yield", fiber_yield, MRB_ARGS_ANY());
  mrb_define_class_method(mrb, c, "current", fiber_current, MRB_ARGS_NONE());

  mrb_define_class(mrb, "FiberError", mrb->eStandardError_class);
}
