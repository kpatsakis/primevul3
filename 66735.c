mrb_obj_freeze(mrb_state *mrb, mrb_value self)
{
  struct RBasic *b;

  switch (mrb_type(self)) {
    case MRB_TT_FALSE:
    case MRB_TT_TRUE:
    case MRB_TT_FIXNUM:
    case MRB_TT_SYMBOL:
#ifndef MRB_WITHOUT_FLOAT
    case MRB_TT_FLOAT:
#endif
      return self;
    default:
      break;
  }

  b = mrb_basic_ptr(self);
  if (!MRB_FROZEN_P(b)) {
    MRB_SET_FROZEN_FLAG(b);
  }
  return self;
}
