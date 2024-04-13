mrb_get_argv(mrb_state *mrb)
{
  mrb_int argc = mrb->c->ci->argc;
  mrb_value *array_argv;
  if (argc < 0) {
    struct RArray *a = mrb_ary_ptr(mrb->c->stack[1]);

    array_argv = ARY_PTR(a);
  }
  else {
    array_argv = NULL;
  }
  return array_argv;
}
