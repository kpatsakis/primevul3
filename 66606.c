const_defined(mrb_state *mrb, mrb_value mod, mrb_sym id, mrb_bool inherit)
{
  if (inherit) {
    return mrb_bool_value(mrb_const_defined(mrb, mod, id));
  }
  return mrb_bool_value(mrb_const_defined_at(mrb, mod, id));
}
