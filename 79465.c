PP_Bool Buffer::IsMapped() {
  return PP_FromBool(map_count_ > 0);
}
