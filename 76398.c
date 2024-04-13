void GLES2DecoderImpl::DoTexParameteri(
    GLenum target, GLenum pname, GLint param) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_VALUE, "glTexParameteri: unknown texture");
  } else {
    texture_manager()->SetParameter(info, pname, param);
    glTexParameteri(target, pname, param);
  }
}
