void GLES2DecoderWithShaderTestBase::DoDeleteProgram(
    GLuint client_id, GLuint service_id) {
  EXPECT_CALL(*gl_, DeleteProgram(service_id))
      .Times(1)
      .RetiresOnSaturation();
  DeleteProgram cmd;
  cmd.Init(client_id);
  EXPECT_EQ(error::kNoError, ExecuteCmd(cmd));
}
