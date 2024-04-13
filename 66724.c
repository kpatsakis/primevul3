mrb_f_raise(mrb_state *mrb, mrb_value self)
{
  mrb_value a[2], exc;
  mrb_int argc;


  argc = mrb_get_args(mrb, "|oo", &a[0], &a[1]);
  switch (argc) {
  case 0:
    mrb_raise(mrb, E_RUNTIME_ERROR, "");
    break;
  case 1:
    if (mrb_string_p(a[0])) {
      a[1] = a[0];
      argc = 2;
      a[0] = mrb_obj_value(E_RUNTIME_ERROR);
    }
    /* fall through */
  default:
    exc = mrb_make_exception(mrb, argc, a);
    mrb_exc_raise(mrb, exc);
    break;
  }
  return mrb_nil_value();            /* not reached */
}
