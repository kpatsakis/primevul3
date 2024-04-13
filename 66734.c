mrb_obj_extend_m(mrb_state *mrb, mrb_value self)
{
  mrb_value *argv;
  mrb_int argc;

  mrb_get_args(mrb, "*", &argv, &argc);
  return mrb_obj_extend(mrb, argc, argv, self);
}
