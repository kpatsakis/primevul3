ScopedRenderBufferBinder::~ScopedRenderBufferBinder() {
  ScopedGLErrorSuppressor suppressor(decoder_);
  glBindRenderbufferEXT(
      GL_RENDERBUFFER,
      decoder_->bound_renderbuffer_ ?
          decoder_->bound_renderbuffer_->service_id() : 0);
}
