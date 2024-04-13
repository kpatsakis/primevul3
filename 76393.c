void GLES2DecoderImpl::DoRenderbufferStorage(
  GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {
  if (!bound_renderbuffer_) {
    SetGLError(GL_INVALID_OPERATION,
               "glGetRenderbufferStorage: no renderbuffer bound");
    return;
  }
  bound_renderbuffer_->set_internal_format(internalformat);

  if (gfx::GetGLImplementation() != gfx::kGLImplementationEGLGLES2) {
    switch (internalformat) {
      case GL_DEPTH_COMPONENT16:
        internalformat = GL_DEPTH_COMPONENT;
        break;
      case GL_RGBA4:
      case GL_RGB5_A1:
        internalformat = GL_RGBA;
        break;
      case GL_RGB565:
        internalformat = GL_RGB;
        break;
    }
  }

  glRenderbufferStorageEXT(target, internalformat, width, height);
}
