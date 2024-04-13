mrb_check_inheritable(mrb_state *mrb, struct RClass *super)
{
  if (super->tt != MRB_TT_CLASS) {
    mrb_raisef(mrb, E_TYPE_ERROR, "superclass must be a Class (%S given)", mrb_obj_value(super));
  }
  if (super->tt == MRB_TT_SCLASS) {
    mrb_raise(mrb, E_TYPE_ERROR, "can't make subclass of singleton class");
  }
  if (super == mrb->class_class) {
    mrb_raise(mrb, E_TYPE_ERROR, "can't make subclass of Class");
  }
}
