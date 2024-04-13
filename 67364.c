argnum_error(mrb_state *mrb, mrb_int num)
{
  mrb_value exc;
  mrb_value str;
  mrb_int argc = mrb->c->ci->argc;

  if (argc < 0) {
    mrb_value args = mrb->c->stack[1];
    if (mrb_array_p(args)) {
      argc = RARRAY_LEN(args);
    }
  }
  if (mrb->c->ci->mid) {
    str = mrb_format(mrb, "'%S': wrong number of arguments (%S for %S)",
                  mrb_sym2str(mrb, mrb->c->ci->mid),
                  mrb_fixnum_value(argc), mrb_fixnum_value(num));
  }
  else {
    str = mrb_format(mrb, "wrong number of arguments (%S for %S)",
                     mrb_fixnum_value(argc), mrb_fixnum_value(num));
  }
  exc = mrb_exc_new_str(mrb, E_ARGUMENT_ERROR, str);
  mrb_exc_set(mrb, exc);
}
