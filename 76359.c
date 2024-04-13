void GLES2DecoderImpl::DeleteRenderbuffersHelper(
    GLsizei n, const GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    RenderbufferManager::RenderbufferInfo* info =
        GetRenderbufferInfo(client_ids[ii]);
    if (info) {
      GLuint service_id = info->service_id();
      glDeleteRenderbuffersEXT(1, &service_id);
      RemoveRenderbufferInfo(client_ids[ii]);
    }
  }
}
