void GLES2DecoderTestBase::DoBindTexture(
    GLenum target, GLuint client_id, GLuint service_id) {
  EXPECT_CALL(*gl_, BindTexture(target, service_id))
      .Times(1)
      .RetiresOnSaturation();
  BindTexture cmd;
  cmd.Init(target, client_id);
  EXPECT_EQ(error::kNoError, ExecuteCmd(cmd));
}
