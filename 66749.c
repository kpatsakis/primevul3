mrb_obj_singleton_methods(mrb_state *mrb, mrb_bool recur, mrb_value obj)
{
  khint_t i;
  mrb_value ary;
  struct RClass* klass;
  khash_t(st)* set = kh_init(st, mrb);

  klass = mrb_class(mrb, obj);

  if (klass && (klass->tt == MRB_TT_SCLASS)) {
      method_entry_loop(mrb, klass, set);
      klass = klass->super;
  }
  if (recur) {
      while (klass && ((klass->tt == MRB_TT_SCLASS) || (klass->tt == MRB_TT_ICLASS))) {
        method_entry_loop(mrb, klass, set);
        klass = klass->super;
      }
  }

  ary = mrb_ary_new(mrb);
  for (i=0;i<kh_end(set);i++) {
    if (kh_exist(set, i)) {
      mrb_ary_push(mrb, ary, mrb_symbol_value(kh_key(set, i)));
    }
  }
  kh_destroy(st, mrb, set);

  return ary;
}
