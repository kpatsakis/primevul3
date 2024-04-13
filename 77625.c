void GLES2DecoderImpl::DoGetRenderbufferParameteriv(
    GLenum target, GLenum pname, GLint* params) {
  RenderbufferManager::RenderbufferInfo* renderbuffer =
      GetRenderbufferInfoForTarget(GL_RENDERBUFFER);
  if (!renderbuffer) {
    SetGLError(GL_INVALID_OPERATION,
               "glGetRenderbufferParameteriv: no renderbuffer bound");
    return;
  }
  switch (pname) {
  case GL_RENDERBUFFER_INTERNAL_FORMAT:
    *params = renderbuffer->internal_format();
    break;
  case GL_RENDERBUFFER_WIDTH:
    *params = renderbuffer->width();
    break;
  case GL_RENDERBUFFER_HEIGHT:
    *params = renderbuffer->height();
    break;
  default:
    glGetRenderbufferParameterivEXT(target, pname, params);
    break;
  }
}
