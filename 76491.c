void GLES2DecoderTestBase::TearDown() {
  EXPECT_EQ(GL_NO_ERROR, GetGLError());
  EXPECT_CALL(*gl_, DeleteTextures(1, _))
      .Times(2)
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, DeleteBuffersARB(1, _))
      .Times(1)
      .RetiresOnSaturation();
  decoder_->Destroy();
  decoder_.reset();
  group_.Destroy(false);
  engine_.reset();
  ::gfx::GLInterface::SetGLInterface(NULL);
  gl_.reset();
}
