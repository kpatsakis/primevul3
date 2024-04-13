bool GLES2DecoderImpl::DoIsFramebuffer(GLuint client_id) {
  return GetFramebufferInfo(client_id) != NULL;
}
