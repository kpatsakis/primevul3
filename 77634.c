void GLES2DecoderImpl::DoTexParameteri(
    GLenum target, GLenum pname, GLint param) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_VALUE, "glTexParameteri: unknown texture");
    return;
  }

  if (!texture_manager()->SetParameter(info, pname, param)) {
    SetGLError(GL_INVALID_ENUM, "glTexParameteri: param GL_INVALID_ENUM");
    return;
  }
  glTexParameteri(target, pname, param);
}
