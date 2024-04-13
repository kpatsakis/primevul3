 void GLES2DecoderImpl::DoGetRenderbufferParameteriv(
    GLenum target, GLenum pname, GLint* params) {
  if (!bound_renderbuffer_) {
    SetGLError(GL_INVALID_OPERATION,
               "glGetRenderbufferParameteriv: no renderbuffer bound");
    return;
  }
  if (pname == GL_RENDERBUFFER_INTERNAL_FORMAT) {
    *params = bound_renderbuffer_->internal_format();
    return;
  }
  glGetRenderbufferParameterivEXT(target, pname, params);
}
