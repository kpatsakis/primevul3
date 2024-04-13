void GLES2DecoderImpl::DoTexParameteriv(
  GLenum target, GLenum pname, const GLint* params) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_VALUE, "glTexParameteriv: unknown texture");
  } else {
    texture_manager()->SetParameter(info, pname, *params);
    glTexParameteriv(target, pname, params);
  }
}
