remove_method(mrb_state *mrb, mrb_value mod, mrb_sym mid)
{
  struct RClass *c = mrb_class_ptr(mod);
  khash_t(mt) *h = find_origin(c)->mt;
  khiter_t k;

  if (h) {
    k = kh_get(mt, mrb, h, mid);
    if (k != kh_end(h)) {
      kh_del(mt, mrb, h, k);
      mrb_funcall(mrb, mod, "method_removed", 1, mrb_symbol_value(mid));
      return;
    }
  }

  mrb_name_error(mrb, mid, "method '%S' not defined in %S",
    mrb_sym2str(mrb, mid), mod);
}
