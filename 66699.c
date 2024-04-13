to_hash(mrb_state *mrb, mrb_value val)
{
  return check_type(mrb, val, MRB_TT_HASH, "Hash", "to_hash");
}
