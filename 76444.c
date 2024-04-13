void GLES2DecoderImpl::SetGLError(GLenum error, const char* msg) {
  if (msg) {
    last_error_ = msg;
    DLOG(ERROR) << last_error_;
  }
  error_bits_ |= GLES2Util::GLErrorToErrorBit(error);
}
