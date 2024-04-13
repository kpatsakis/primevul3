void GLES2DecoderImpl::DeleteFramebuffersHelper(
    GLsizei n, const GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    FramebufferManager::FramebufferInfo* info =
        GetFramebufferInfo(client_ids[ii]);
    if (info) {
      GLuint service_id = info->service_id();
      glDeleteFramebuffersEXT(1, &service_id);
      RemoveFramebufferInfo(client_ids[ii]);
    }
  }
}
