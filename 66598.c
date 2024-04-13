boot_initmod(mrb_state *mrb, struct RClass *mod)
{
  if (!mod->mt) {
    mod->mt = kh_init(mt, mrb);
  }
}
