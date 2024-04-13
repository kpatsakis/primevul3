ScopedDefaultGLContext::ScopedDefaultGLContext(GLES2DecoderImpl* decoder)
    : decoder_(decoder) {
  decoder_->default_context_->MakeCurrent();
}
