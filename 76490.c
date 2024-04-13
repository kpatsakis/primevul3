void GLES2DecoderWithShaderTestBase::SetupVertexBuffer() {
  DoEnableVertexAttribArray(1);
  DoBindBuffer(GL_ARRAY_BUFFER, client_buffer_id_, kServiceBufferId);
  GLfloat f = 0;
  DoBufferData(GL_ARRAY_BUFFER, kNumVertices * 2 * sizeof(f));
}
