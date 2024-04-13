bool Texture::AllocateStorage(const gfx::Size& size) {
  DCHECK_NE(id_, 0u);
  ScopedGLErrorSuppressor suppressor(decoder_);
  ScopedTexture2DBinder binder(decoder_, id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D,
               0,  // mip level
               GL_RGBA,
               size.width(),
               size.height(),
               0,  // border
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               NULL);

  size_ = size;

  return glGetError() == GL_NO_ERROR;
}
