void GLES2DecoderTestBase::DoDeleteBuffer(
    GLuint client_id, GLuint service_id) {
  EXPECT_CALL(*gl_, DeleteBuffersARB(1, Pointee(service_id)))
      .Times(1)
      .RetiresOnSaturation();
  DeleteBuffers cmd;
  cmd.Init(1, shared_memory_id_, shared_memory_offset_);
  memcpy(shared_memory_address_, &client_id, sizeof(client_id));
  EXPECT_EQ(error::kNoError, ExecuteCmd(cmd));
}
