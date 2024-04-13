bool RenderBuffer::AllocateStorage(const gfx::Size& size, GLenum format) {
  ScopedGLErrorSuppressor suppressor(decoder_);
  ScopedRenderBufferBinder binder(decoder_, id_);
  glRenderbufferStorageEXT(GL_RENDERBUFFER,
                           format,
                           size.width(),
                           size.height());
  return glGetError() == GL_NO_ERROR;
}
