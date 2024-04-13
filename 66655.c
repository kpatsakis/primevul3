mrb_method_search(mrb_state *mrb, struct RClass* c, mrb_sym mid)
{
  mrb_method_t m;

  m = mrb_method_search_vm(mrb, &c, mid);
  if (MRB_METHOD_UNDEF_P(m)) {
    mrb_value inspect = mrb_funcall(mrb, mrb_obj_value(c), "inspect", 0);
    if (mrb_string_p(inspect) && RSTRING_LEN(inspect) > 64) {
      inspect = mrb_any_to_s(mrb, mrb_obj_value(c));
    }
    mrb_name_error(mrb, mid, "undefined method '%S' for class %S",
               mrb_sym2str(mrb, mid), inspect);
  }
  return m;
}
