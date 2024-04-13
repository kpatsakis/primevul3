mrb_define_alias(mrb_state *mrb, struct RClass *klass, const char *name1, const char *name2)
{
  mrb_alias_method(mrb, klass, mrb_intern_cstr(mrb, name1), mrb_intern_cstr(mrb, name2));
}
