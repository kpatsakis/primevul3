mrb_method_search_vm(mrb_state *mrb, struct RClass **cp, mrb_sym mid)
{
  khiter_t k;
  mrb_method_t m;
  struct RClass *c = *cp;
#ifdef MRB_METHOD_CACHE
  struct RClass *oc = c;
  int h = kh_int_hash_func(mrb, ((intptr_t)oc) ^ mid) & (MRB_METHOD_CACHE_SIZE-1);
  struct mrb_cache_entry *mc = &mrb->cache[h];

  if (mc->c == c && mc->mid == mid) {
    *cp = mc->c0;
    return mc->m;
  }
#endif

  while (c) {
    khash_t(mt) *h = c->mt;

    if (h) {
      k = kh_get(mt, mrb, h, mid);
      if (k != kh_end(h)) {
        m = kh_value(h, k);
        if (MRB_METHOD_UNDEF_P(m)) break;
        *cp = c;
#ifdef MRB_METHOD_CACHE
        mc->c = oc;
        mc->c0 = c;
        mc->mid = mid;
        mc->m = m;
#endif
        return m;
      }
    }
    c = c->super;
  }
  MRB_METHOD_FROM_PROC(m, NULL);
  return m;                  /* no method */
}
