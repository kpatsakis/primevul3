void GLES2DecoderImpl::DeleteTexturesHelper(
    GLsizei n, const GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    TextureManager::TextureInfo* info = GetTextureInfo(client_ids[ii]);
    if (info) {
      GLuint service_id = info->service_id();
      glDeleteTextures(1, &service_id);
      RemoveTextureInfo(client_ids[ii]);
    }
  }
}
