class_ptr_p(mrb_value obj)
{
  switch (mrb_type(obj)) {
  case MRB_TT_CLASS:
  case MRB_TT_SCLASS:
  case MRB_TT_MODULE:
    return TRUE;
  default:
    return FALSE;
  }
}
