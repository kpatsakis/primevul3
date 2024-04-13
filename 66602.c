check_cv_name_sym(mrb_state *mrb, mrb_sym id)
{
  check_cv_name_str(mrb, mrb_sym2str(mrb, id));
}
