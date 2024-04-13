mrb_class_name_class(mrb_state *mrb, struct RClass *outer, struct RClass *c, mrb_sym id)
{
  mrb_value name;
  mrb_sym nsym = mrb_intern_lit(mrb, "__classname__");

  if (mrb_obj_iv_defined(mrb, (struct RObject*)c, nsym)) return;
  if (outer == NULL || outer == mrb->object_class) {
    name = mrb_symbol_value(id);
  }
  else {
    name = mrb_class_path(mrb, outer);
    if (mrb_nil_p(name)) {      /* unnamed outer class */
      if (outer != mrb->object_class) {
        mrb_obj_iv_set(mrb, (struct RObject*)c, mrb_intern_lit(mrb, "__outer__"),
                       mrb_obj_value(outer));
      }
      return;
    }
    mrb_str_cat_cstr(mrb, name, "::");
    mrb_str_cat_cstr(mrb, name, mrb_sym2name(mrb, id));
  }
  mrb_obj_iv_set(mrb, (struct RObject*)c, nsym, name);
}
