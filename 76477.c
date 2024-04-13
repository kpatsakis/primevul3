void GLES2DecoderTestBase::DoCreateShader(
    GLenum shader_type, GLuint client_id, GLuint service_id) {
  EXPECT_CALL(*gl_, CreateShader(shader_type))
      .Times(1)
      .WillOnce(Return(service_id))
      .RetiresOnSaturation();
  CreateShader cmd;
  cmd.Init(shader_type, client_id);
  EXPECT_EQ(error::kNoError, ExecuteCmd(cmd));
}
