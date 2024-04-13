mrb_obj_respond_to(mrb_state *mrb, struct RClass* c, mrb_sym mid)
{
  mrb_method_t m;

  m = mrb_method_search_vm(mrb, &c, mid);
  if (MRB_METHOD_UNDEF_P(m)) {
    return FALSE;
  }
  return TRUE;
}
