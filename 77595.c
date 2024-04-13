bool Texture::AllocateStorage(const gfx::Size& size, GLenum format) {
  DCHECK_NE(id_, 0u);
  ScopedGLErrorSuppressor suppressor(decoder_);
  ScopedTexture2DBinder binder(decoder_, id_);

  WrappedTexImage2D(GL_TEXTURE_2D,
                    0,  // mip level
                    format,
                    size.width(),
                    size.height(),
                    0,  // border
                    format,
                    GL_UNSIGNED_BYTE,
                    NULL);

  size_ = size;

  bool success = glGetError() == GL_NO_ERROR;
  if (success) {
    uint32 image_size = 0;
    GLES2Util::ComputeImageDataSize(
        size.width(), size.height(), format, GL_UNSIGNED_BYTE, 4, &image_size);
    estimated_size_ = image_size;
    decoder_->UpdateBackbufferMemoryAccounting();
  }
  return success;
}
