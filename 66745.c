mrb_obj_missing(mrb_state *mrb, mrb_value mod)
{
  mrb_sym name;
  mrb_value *a;
  mrb_int alen;

  mrb_get_args(mrb, "n*!", &name, &a, &alen);
  mrb_method_missing(mrb, name, mod, mrb_ary_new_from_values(mrb, alen, a));
  /* not reached */
  return mrb_nil_value();
}
