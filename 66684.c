mrb_module_new(mrb_state *mrb)
{
  struct RClass *m = (struct RClass*)mrb_obj_alloc(mrb, MRB_TT_MODULE, mrb->module_class);
  boot_initmod(mrb, m);
  return m;
}
