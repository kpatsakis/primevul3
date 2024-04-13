mrb_mod_attr_writer(mrb_state *mrb, mrb_value mod)
{
  struct RClass *c = mrb_class_ptr(mod);
  mrb_value *argv;
  mrb_int argc, i;
  int ai;

  mrb_get_args(mrb, "*", &argv, &argc);
  ai = mrb_gc_arena_save(mrb);
  for (i=0; i<argc; i++) {
    mrb_value name, str, attr;
    mrb_sym method, sym;
    struct RProc *p;
    mrb_method_t m;

    method = to_sym(mrb, argv[i]);

    /* prepare iv name (@name) */
    name = mrb_sym2str(mrb, method);
    str = mrb_str_new_capa(mrb, RSTRING_LEN(name)+1);
    mrb_str_cat_lit(mrb, str, "@");
    mrb_str_cat_str(mrb, str, name);
    sym = mrb_intern_str(mrb, str);
    mrb_iv_check(mrb, sym);
    attr = mrb_symbol_value(sym);

    /* prepare method name (name=) */
    str = mrb_str_new_capa(mrb, RSTRING_LEN(str));
    mrb_str_cat_str(mrb, str, name);
    mrb_str_cat_lit(mrb, str, "=");
    method = mrb_intern_str(mrb, str);

    p = mrb_proc_new_cfunc_with_env(mrb, attr_writer, 1, &attr);
    MRB_METHOD_FROM_PROC(m, p);
    mrb_define_method_raw(mrb, c, method, m);
    mrb_gc_arena_restore(mrb, ai);
  }
  return mrb_nil_value();
}
