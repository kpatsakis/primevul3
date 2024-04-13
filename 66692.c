mrb_singleton_class(mrb_state *mrb, mrb_value v)
{
  struct RBasic *obj;

  switch (mrb_type(v)) {
  case MRB_TT_FALSE:
    if (mrb_nil_p(v))
      return mrb_obj_value(mrb->nil_class);
    return mrb_obj_value(mrb->false_class);
  case MRB_TT_TRUE:
    return mrb_obj_value(mrb->true_class);
  case MRB_TT_CPTR:
    return mrb_obj_value(mrb->object_class);
  case MRB_TT_SYMBOL:
  case MRB_TT_FIXNUM:
#ifndef MRB_WITHOUT_FLOAT
  case MRB_TT_FLOAT:
#endif
    mrb_raise(mrb, E_TYPE_ERROR, "can't define singleton");
    return mrb_nil_value();    /* not reached */
  default:
    break;
  }
  obj = mrb_basic_ptr(v);
  prepare_singleton_class(mrb, obj);
  return mrb_obj_value(obj->c);
}
