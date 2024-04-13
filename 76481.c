void GLES2DecoderTestBase::DoVertexAttribPointer(
    GLuint index, GLint size, GLenum type, GLsizei stride, GLuint offset) {
  EXPECT_CALL(*gl_,
              VertexAttribPointer(index, size, type, GL_FALSE, stride,
                                  BufferOffset(offset)))
      .Times(1)
      .RetiresOnSaturation();
  VertexAttribPointer cmd;
  cmd.Init(index, size, GL_FLOAT, GL_FALSE, stride, offset);
  EXPECT_EQ(error::kNoError, ExecuteCmd(cmd));
}
