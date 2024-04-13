mrb_method_missing(mrb_state *mrb, mrb_sym name, mrb_value self, mrb_value args)
{
  mrb_no_method_error(mrb, name, args, "undefined method '%S'", mrb_sym2str(mrb, name));
}
