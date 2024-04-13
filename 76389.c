bool GLES2DecoderImpl::DoIsRenderbuffer(GLuint client_id) {
  return GetRenderbufferInfo(client_id) != NULL;
}
