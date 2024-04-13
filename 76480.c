void GLES2DecoderTestBase::DoTexImage2D(
    GLenum target, GLint level, GLenum internal_format,
    GLsizei width, GLsizei height, GLint border,
    GLenum format, GLenum type,
    uint32 shared_memory_id, uint32 shared_memory_offset) {
  EXPECT_CALL(*gl_, GetError())
      .WillOnce(Return(GL_NO_ERROR))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, TexImage2D(target, level, internal_format,
                               width, height, border, format, type, _))
      .Times(1)
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, GetError())
      .WillOnce(Return(GL_NO_ERROR))
      .RetiresOnSaturation();
  TexImage2D cmd;
  cmd.Init(target, level, internal_format, width, height, border, format,
           type, shared_memory_id, shared_memory_offset);
  EXPECT_EQ(error::kNoError, ExecuteCmd(cmd));
}
