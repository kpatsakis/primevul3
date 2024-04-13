void RenderBuffer::Destroy() {
  if (id_ != 0) {
    ScopedGLErrorSuppressor suppressor(decoder_);
    glDeleteRenderbuffersEXT(1, &id_);
    id_ = 0;
    estimated_size_ = 0;
    decoder_->UpdateBackbufferMemoryAccounting();
  }
}
