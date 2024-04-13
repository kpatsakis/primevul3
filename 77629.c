void GLES2DecoderImpl::DoRenderbufferStorageMultisample(
    GLenum target, GLsizei samples, GLenum internalformat,
    GLsizei width, GLsizei height) {
  if (!feature_info_->feature_flags().chromium_framebuffer_multisample) {
    SetGLError(GL_INVALID_OPERATION,
               "glRenderbufferStorageMultisampleEXT: function not available");
    return;
  }

  RenderbufferManager::RenderbufferInfo* renderbuffer =
      GetRenderbufferInfoForTarget(GL_RENDERBUFFER);
  if (!renderbuffer) {
    SetGLError(GL_INVALID_OPERATION,
               "glGetRenderbufferStorageMultisample: no renderbuffer bound");
    return;
  }

  if (samples > renderbuffer_manager()->max_samples()) {
    SetGLError(GL_INVALID_VALUE,
               "glGetRenderbufferStorageMultisample: samples too large");
    return;
  }

  if (width > renderbuffer_manager()->max_renderbuffer_size() ||
      height > renderbuffer_manager()->max_renderbuffer_size()) {
    SetGLError(GL_INVALID_VALUE,
               "glGetRenderbufferStorageMultisample: size too large");
    return;
  }

  GLenum impl_format = internalformat;
  if (gfx::GetGLImplementation() != gfx::kGLImplementationEGLGLES2) {
    switch (impl_format) {
      case GL_DEPTH_COMPONENT16:
        impl_format = GL_DEPTH_COMPONENT;
        break;
      case GL_RGBA4:
      case GL_RGB5_A1:
        impl_format = GL_RGBA;
        break;
      case GL_RGB565:
        impl_format = GL_RGB;
        break;
    }
  }

  CopyRealGLErrorsToWrapper();
  if (IsAngle()) {
    glRenderbufferStorageMultisampleANGLE(
        target, samples, impl_format, width, height);
  } else {
    glRenderbufferStorageMultisampleEXT(
        target, samples, impl_format, width, height);
  }
  GLenum error = PeekGLError();
  if (error == GL_NO_ERROR) {
    framebuffer_manager()->IncFramebufferStateChangeCount();
    renderbuffer_manager()->SetInfo(
        renderbuffer, samples, internalformat, width, height);
  }
}
