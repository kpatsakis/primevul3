mrb_notimplement(mrb_state *mrb)
{
  const char *str;
  mrb_int len;
  mrb_callinfo *ci = mrb->c->ci;

  if (ci->mid) {
    str = mrb_sym2name_len(mrb, ci->mid, &len);
    mrb_raisef(mrb, E_NOTIMP_ERROR,
      "%S() function is unimplemented on this machine",
      mrb_str_new_static(mrb, str, (size_t)len));
  }
}
