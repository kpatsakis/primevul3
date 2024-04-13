void GLES2DecoderTestBase::DoBindRenderbuffer(
    GLenum target, GLuint client_id, GLuint service_id) {
  EXPECT_CALL(*gl_, BindRenderbufferEXT(target, service_id))
      .Times(1)
      .RetiresOnSaturation();
  BindRenderbuffer cmd;
  cmd.Init(target, client_id);
  EXPECT_EQ(error::kNoError, ExecuteCmd(cmd));
}
