void GLES2DecoderImpl::DoTexParameterfv(
    GLenum target, GLenum pname, const GLfloat* params) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_VALUE, "glTexParameterfv: unknown texture");
  } else {
    texture_manager()->SetParameter(
        info, pname, *reinterpret_cast<const GLint*>(params));
    glTexParameterfv(target, pname, params);
  }
}
