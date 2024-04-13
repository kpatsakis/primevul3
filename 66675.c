mrb_mod_module_function(mrb_state *mrb, mrb_value mod)
{
  mrb_value *argv;
  mrb_int argc, i;
  mrb_sym mid;
  mrb_method_t m;
  struct RClass *rclass;
  int ai;

  mrb_check_type(mrb, mod, MRB_TT_MODULE);

  mrb_get_args(mrb, "*", &argv, &argc);
  if (argc == 0) {
    /* set MODFUNC SCOPE if implemented */
    return mod;
  }

  /* set PRIVATE method visibility if implemented */
  /* mrb_mod_dummy_visibility(mrb, mod); */

  for (i=0; i<argc; i++) {
    mrb_check_type(mrb, argv[i], MRB_TT_SYMBOL);

    mid = mrb_symbol(argv[i]);
    rclass = mrb_class_ptr(mod);
    m = mrb_method_search(mrb, rclass, mid);

    prepare_singleton_class(mrb, (struct RBasic*)rclass);
    ai = mrb_gc_arena_save(mrb);
    mrb_define_method_raw(mrb, rclass->c, mid, m);
    mrb_gc_arena_restore(mrb, ai);
  }

  return mod;
}
