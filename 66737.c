mrb_obj_hash(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_obj_id(self));
}
