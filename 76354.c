void Texture::Copy(const gfx::Size& size) {
  DCHECK_NE(id_, 0u);
  ScopedGLErrorSuppressor suppressor(decoder_);
  ScopedTexture2DBinder binder(decoder_, id_);
  glCopyTexImage2D(GL_TEXTURE_2D,
                   0,  // level
                   GL_RGBA,
                   0, 0,
                   size.width(),
                   size.height(),
                   0);  // border
}
