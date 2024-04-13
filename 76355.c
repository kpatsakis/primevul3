void Texture::Create() {
  ScopedGLErrorSuppressor suppressor(decoder_);
  Destroy();
  glGenTextures(1, &id_);
}
