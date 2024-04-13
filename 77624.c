void GLES2DecoderImpl::DoGenerateMipmap(GLenum target) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info ||
      !texture_manager()->MarkMipmapsGenerated(info)) {
    SetGLError(GL_INVALID_OPERATION,
               "glGenerateMipmaps: Can not generate mips");
    return;
  }
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glGenerateMipmapEXT(target);
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, info->min_filter());
}
