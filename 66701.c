to_sym(mrb_state *mrb, mrb_value ss)
{
  if (mrb_type(ss) == MRB_TT_SYMBOL) {
    return mrb_symbol(ss);
  }
  else if (mrb_string_p(ss)) {
    return mrb_intern_str(mrb, to_str(mrb, ss));
  }
  else {
    mrb_value obj = mrb_funcall(mrb, ss, "inspect", 0);
    mrb_raisef(mrb, E_TYPE_ERROR, "%S is not a symbol", obj);
    /* not reached */
    return 0;
  }
}
