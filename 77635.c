void GLES2DecoderImpl::DoTexParameteriv(
  GLenum target, GLenum pname, const GLint* params) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_VALUE, "glTexParameteriv: unknown texture");
    return;
  }

  if (!texture_manager()->SetParameter(info, pname, *params)) {
    SetGLError(GL_INVALID_ENUM, "glTexParameteriv: param GL_INVALID_ENUM");
    return;
  }
  glTexParameteriv(target, pname, params);
}
