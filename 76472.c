void GLES2DecoderTestBase::DoBindBuffer(
    GLenum target, GLuint client_id, GLuint service_id) {
  EXPECT_CALL(*gl_, BindBuffer(target, service_id))
      .Times(1)
      .RetiresOnSaturation();
  BindBuffer cmd;
  cmd.Init(target, client_id);
  EXPECT_EQ(error::kNoError, ExecuteCmd(cmd));
}
