mrb_obj_extend(mrb_state *mrb, mrb_int argc, mrb_value *argv, mrb_value obj)
{
  mrb_int i;

  if (argc == 0) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments (at least 1)");
  }
  for (i = 0; i < argc; i++) {
    mrb_check_type(mrb, argv[i], MRB_TT_MODULE);
  }
  while (argc--) {
    mrb_funcall(mrb, argv[argc], "extend_object", 1, obj);
    mrb_funcall(mrb, argv[argc], "extended", 1, obj);
  }
  return obj;
}
