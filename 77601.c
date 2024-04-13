void Texture::Create() {
  ScopedGLErrorSuppressor suppressor(decoder_);
  Destroy();
  glGenTextures(1, &id_);
  ScopedTexture2DBinder binder(decoder_, id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  estimated_size_ = 16u * 16u * 4u;
  decoder_->UpdateBackbufferMemoryAccounting();
}
