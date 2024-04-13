bool GLES2DecoderImpl::CheckFramebufferValid(
    FramebufferManager::FramebufferInfo* framebuffer,
    GLenum target, const char* func_name) {
  if (!framebuffer) {
    return true;
  }

  if (framebuffer_manager()->IsComplete(framebuffer)) {
    return true;
  }

  GLenum completeness = framebuffer->IsPossiblyComplete();
  if (completeness != GL_FRAMEBUFFER_COMPLETE) {
    SetGLError(
        GL_INVALID_FRAMEBUFFER_OPERATION,
        (std::string(func_name) + " framebuffer incomplete").c_str());
    return false;
  }

  if (renderbuffer_manager()->HaveUnclearedRenderbuffers() ||
      texture_manager()->HaveUnclearedMips()) {
    if (!framebuffer->IsCleared()) {
      if (glCheckFramebufferStatusEXT(target) != GL_FRAMEBUFFER_COMPLETE) {
        SetGLError(
            GL_INVALID_FRAMEBUFFER_OPERATION,
            (std::string(func_name) +
            " framebuffer incomplete (clear)").c_str());
        return false;
      }
      ClearUnclearedAttachments(target, framebuffer);
    }
  }

  if (!framebuffer_manager()->IsComplete(framebuffer)) {
    if (glCheckFramebufferStatusEXT(target) != GL_FRAMEBUFFER_COMPLETE) {
      SetGLError(
          GL_INVALID_FRAMEBUFFER_OPERATION,
          (std::string(func_name) +
          " framebuffer incomplete (check)").c_str());
      return false;
    }
    framebuffer_manager()->MarkAsComplete(framebuffer);
  }

  return true;
}
