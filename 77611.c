void GLES2DecoderImpl::DoBlitFramebufferEXT(
    GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
    GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
    GLbitfield mask, GLenum filter) {
  if (!feature_info_->feature_flags().chromium_framebuffer_multisample) {
    SetGLError(GL_INVALID_OPERATION,
               "glBlitFramebufferEXT: function not available");
  }
  if (IsAngle()) {
    glBlitFramebufferANGLE(
        srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
  } else {
    glBlitFramebufferEXT(
        srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
  }
  UNSHIPPED_TRACE_EVENT_INSTANT1("test_gpu", "DoBlit", "width", srcX1 - srcX0);
}
