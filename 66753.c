obj_respond_to(mrb_state *mrb, mrb_value self)
{
  mrb_value mid;
  mrb_sym id, rtm_id;
  mrb_bool priv = FALSE, respond_to_p = TRUE;

  mrb_get_args(mrb, "o|b", &mid, &priv);

  if (mrb_symbol_p(mid)) {
    id = mrb_symbol(mid);
  }
  else {
    mrb_value tmp;
    if (mrb_string_p(mid)) {
      tmp = mrb_check_intern_str(mrb, mid);
    }
    else {
      tmp = mrb_check_string_type(mrb, mid);
      if (mrb_nil_p(tmp)) {
        tmp = mrb_inspect(mrb, mid);
        mrb_raisef(mrb, E_TYPE_ERROR, "%S is not a symbol", tmp);
      }
      tmp = mrb_check_intern_str(mrb, tmp);
    }
    if (mrb_nil_p(tmp)) {
      respond_to_p = FALSE;
    }
    else {
      id = mrb_symbol(tmp);
    }
  }

  if (respond_to_p) {
    respond_to_p = basic_obj_respond_to(mrb, self, id, !priv);
  }

  if (!respond_to_p) {
    rtm_id = mrb_intern_lit(mrb, "respond_to_missing?");
    if (basic_obj_respond_to(mrb, self, rtm_id, !priv)) {
      mrb_value args[2], v;
      args[0] = mid;
      args[1] = mrb_bool_value(priv);
      v = mrb_funcall_argv(mrb, self, rtm_id, 2, args);
      return mrb_bool_value(mrb_bool(v));
    }
  }
  return mrb_bool_value(respond_to_p);
}
