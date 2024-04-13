void GLES2DecoderImpl::SetCapabilityState(GLenum cap, bool enabled) {
  switch (cap) {
    case GL_SCISSOR_TEST:
      enable_scissor_test_ = enabled;
      break;
    default:
      break;
  }
}
