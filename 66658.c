mrb_mod_ancestors(mrb_state *mrb, mrb_value self)
{
  mrb_value result;
  struct RClass *c = mrb_class_ptr(self);
  result = mrb_ary_new(mrb);
  while (c) {
    if (c->tt == MRB_TT_ICLASS) {
      mrb_ary_push(mrb, result, mrb_obj_value(c->c));
    }
    else if (!(c->flags & MRB_FLAG_IS_PREPENDED)) {
      mrb_ary_push(mrb, result, mrb_obj_value(c));
    }
    c = c->super;
  }

  return result;
}
