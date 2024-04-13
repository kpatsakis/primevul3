bool GLES2DecoderImpl::DoIsTexture(GLuint client_id) {
  return GetTextureInfo(client_id) != NULL;
}
