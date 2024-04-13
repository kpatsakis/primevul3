method_entry_loop(mrb_state *mrb, struct RClass* klass, khash_t(st)* set)
{
  khint_t i;

  khash_t(mt) *h = klass->mt;
  if (!h || kh_size(h) == 0) return;
  for (i=0;i<kh_end(h);i++) {
    if (kh_exist(h, i)) {
      mrb_method_t m = kh_value(h, i);
      if (MRB_METHOD_UNDEF_P(m)) continue;
      kh_put(st, mrb, set, kh_key(h, i));
    }
  }
}
