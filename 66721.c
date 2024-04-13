mrb_class_instance_method_list(mrb_state *mrb, mrb_bool recur, struct RClass* klass, int obj)
{
  khint_t i;
  mrb_value ary;
  mrb_bool prepended = FALSE;
  struct RClass* oldklass;
  khash_t(st)* set = kh_init(st, mrb);

  if (!recur && (klass->flags & MRB_FLAG_IS_PREPENDED)) {
    MRB_CLASS_ORIGIN(klass);
    prepended = TRUE;
  }

  oldklass = 0;
  while (klass && (klass != oldklass)) {
    method_entry_loop(mrb, klass, set);
    if ((klass->tt == MRB_TT_ICLASS && !prepended) ||
        (klass->tt == MRB_TT_SCLASS)) {
    }
    else {
      if (!recur) break;
    }
    oldklass = klass;
    klass = klass->super;
  }

  ary = mrb_ary_new_capa(mrb, kh_size(set));
  for (i=0;i<kh_end(set);i++) {
    if (kh_exist(set, i)) {
      mrb_ary_push(mrb, ary, mrb_symbol_value(kh_key(set, i)));
    }
  }
  kh_destroy(st, mrb, set);

  return ary;
}
