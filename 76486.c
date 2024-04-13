void GLES2DecoderWithShaderTestBase::SetupIndexBuffer() {
  DoBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
               client_element_buffer_id_,
               kServiceElementBufferId);
  static const GLshort indices[] = {100, 1, 2, 3, 4, 5, 6, 7, 100, 9};
  COMPILE_ASSERT(arraysize(indices) == kNumIndices, Indices_is_not_10);
  DoBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices));
  DoBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);
}
