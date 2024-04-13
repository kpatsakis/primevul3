include_module_at(mrb_state *mrb, struct RClass *c, struct RClass *ins_pos, struct RClass *m, int search_super)
{
  struct RClass *p, *ic;
  void *klass_mt = find_origin(c)->mt;

  while (m) {
    int superclass_seen = 0;

    if (m->flags & MRB_FLAG_IS_PREPENDED)
      goto skip;

    if (klass_mt && klass_mt == m->mt)
      return -1;

    p = c->super;
    while (p) {
      if (p->tt == MRB_TT_ICLASS) {
        if (p->mt == m->mt) {
          if (!superclass_seen) {
            ins_pos = p; /* move insert point */
          }
          goto skip;
        }
      } else if (p->tt == MRB_TT_CLASS) {
        if (!search_super) break;
        superclass_seen = 1;
      }
      p = p->super;
    }

    ic = include_class_new(mrb, m, ins_pos->super);
    m->flags |= MRB_FLAG_IS_INHERITED;
    ins_pos->super = ic;
    mrb_field_write_barrier(mrb, (struct RBasic*)ins_pos, (struct RBasic*)ic);
    mc_clear_by_class(mrb, ins_pos);
    ins_pos = ic;
  skip:
    m = m->super;
  }
  mc_clear_all(mrb);
  return 0;
}
