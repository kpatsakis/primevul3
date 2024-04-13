mrb_mod_to_s(mrb_state *mrb, mrb_value klass)
{
  mrb_value str;

  if (mrb_type(klass) == MRB_TT_SCLASS) {
    mrb_value v = mrb_iv_get(mrb, klass, mrb_intern_lit(mrb, "__attached__"));

    str = mrb_str_new_lit(mrb, "#<Class:");

    if (class_ptr_p(v)) {
      mrb_str_cat_str(mrb, str, mrb_inspect(mrb, v));
    }
    else {
      mrb_str_cat_str(mrb, str, mrb_any_to_s(mrb, v));
    }
    return mrb_str_cat_lit(mrb, str, ">");
  }
  else {
    struct RClass *c;
    mrb_value path;

    str = mrb_str_new_capa(mrb, 32);
    c = mrb_class_ptr(klass);
    path = mrb_class_path(mrb, c);

    if (mrb_nil_p(path)) {
      switch (mrb_type(klass)) {
        case MRB_TT_CLASS:
          mrb_str_cat_lit(mrb, str, "#<Class:");
          break;

        case MRB_TT_MODULE:
          mrb_str_cat_lit(mrb, str, "#<Module:");
          break;

        default:
          /* Shouldn't be happened? */
          mrb_str_cat_lit(mrb, str, "#<??????:");
          break;
      }
      mrb_str_concat(mrb, str, mrb_ptr_to_str(mrb, c));
      return mrb_str_cat_lit(mrb, str, ">");
    }
    else {
      return path;
    }
  }
}
