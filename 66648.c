mrb_get_argc(mrb_state *mrb)
{
  mrb_int argc = mrb->c->ci->argc;

  if (argc < 0) {
    struct RArray *a = mrb_ary_ptr(mrb->c->stack[1]);

    argc = ARY_LEN(a);
  }
  return argc;
}
