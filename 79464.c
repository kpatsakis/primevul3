PP_Bool Buffer::Describe(uint32_t* size_in_bytes) {
  *size_in_bytes = size_;
  return PP_TRUE;
}
