void GLES2DecoderImpl::ClearUnclearedRenderbuffers(
    FramebufferManager::FramebufferInfo* info) {
  GLbitfield clear_bits = 0;
  if (info->HasUnclearedAttachment(GL_COLOR_ATTACHMENT0)) {
    glClearColor(0, 0, 0, 0);
    glColorMask(true, true, true, true);
    clear_bits |= GL_COLOR_BUFFER_BIT;
  }

  if (info->HasUnclearedAttachment(GL_STENCIL_ATTACHMENT) ||
      info->HasUnclearedAttachment(GL_DEPTH_STENCIL_ATTACHMENT)) {
    glClearStencil(0);
    glStencilMask(-1);
    clear_bits |= GL_STENCIL_BUFFER_BIT;
  }

  if (info->HasUnclearedAttachment(GL_DEPTH_ATTACHMENT) ||
      info->HasUnclearedAttachment(GL_DEPTH_STENCIL_ATTACHMENT)) {
    glClearDepth(1.0f);
    glDepthMask(true);
    clear_bits |= GL_DEPTH_BUFFER_BIT;
  }

  glDisable(GL_SCISSOR_TEST);
  glClear(clear_bits);

  info->MarkAttachedRenderbuffersAsCleared();

  glClearColor(clear_red_, clear_green_, clear_blue_, clear_alpha_);
  glColorMask(mask_red_, mask_green_, mask_blue_, mask_alpha_);
  glClearStencil(clear_stencil_);
  glStencilMaskSeparate(GL_FRONT, mask_stencil_front_);
  glStencilMaskSeparate(GL_BACK, mask_stencil_back_);
  glClearDepth(clear_depth_);
  glDepthMask(mask_depth_);
  if (enable_scissor_test_) {
    glEnable(GL_SCISSOR_TEST);
  }
}
