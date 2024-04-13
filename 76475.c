void GLES2DecoderWithShaderTestBase::DoBufferData(GLenum target, GLsizei size) {
  EXPECT_CALL(*gl_, GetError())
      .WillOnce(Return(GL_NO_ERROR))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, BufferData(target, size, _, GL_STREAM_DRAW))
      .Times(1)
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, GetError())
      .WillOnce(Return(GL_NO_ERROR))
      .RetiresOnSaturation();
  BufferData cmd;
  cmd.Init(target, size, 0, 0, GL_STREAM_DRAW);
  EXPECT_EQ(error::kNoError, ExecuteCmd(cmd));
}
