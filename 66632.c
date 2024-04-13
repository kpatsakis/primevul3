mrb_class_path(mrb_state *mrb, struct RClass *c)
{
  mrb_value path;
  mrb_sym nsym = mrb_intern_lit(mrb, "__classname__");

  path = mrb_obj_iv_get(mrb, (struct RObject*)c, nsym);
  if (mrb_nil_p(path)) {
    /* no name (yet) */
    return mrb_class_find_path(mrb, c);
  }
  else if (mrb_symbol_p(path)) {
    /* toplevel class/module */
    const char *str;
    mrb_int len;

    str = mrb_sym2name_len(mrb, mrb_symbol(path), &len);
    return mrb_str_new(mrb, str, len);
  }
  return mrb_str_dup(mrb, path);
}
