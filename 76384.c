void GLES2DecoderImpl::DoGenerateMipmap(GLenum target) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info || !texture_manager()->MarkMipmapsGenerated(info)) {
    SetGLError(GL_INVALID_OPERATION,
               "glGenerateMipmaps: Can not generate mips for npot textures");
    return;
  }
  glGenerateMipmapEXT(target);
}
