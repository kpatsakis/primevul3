void GLES2DecoderImpl::DoDisable(GLenum cap) {
  SetCapabilityState(cap, false);
  glDisable(cap);
}
