option_to_fd(mrb_state *mrb, mrb_value obj, const char *key)
{
  mrb_value opt = mrb_funcall(mrb, obj, "[]", 1, mrb_symbol_value(mrb_intern_static(mrb, key, strlen(key))));
  if (mrb_nil_p(opt)) {
    return -1;
  }

  switch (mrb_type(opt)) {
    case MRB_TT_DATA: /* IO */
      return (int)mrb_fixnum(mrb_io_fileno(mrb, opt));
    case MRB_TT_FIXNUM:
      return (int)mrb_fixnum(opt);
    default:
      mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong exec redirect action");
      break;
  }
  return -1; /* never reached */
}
