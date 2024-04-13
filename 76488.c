 void GLES2DecoderTestBase::SetupShaderForUniform() {
   static AttribInfo attribs[] = {
     { "foo", 1, GL_FLOAT, 1, },
  };
  static UniformInfo uniforms[] = {
    { "bar", 1, GL_INT, 1, },
  };
  const GLuint kClientVertexShaderId = 5001;
  const GLuint kServiceVertexShaderId = 6001;
  const GLuint kClientFragmentShaderId = 5002;
  const GLuint kServiceFragmentShaderId = 6002;
  SetupShader(attribs, arraysize(attribs), uniforms, arraysize(uniforms),
              client_program_id_, kServiceProgramId,
              kClientVertexShaderId, kServiceVertexShaderId,
              kClientFragmentShaderId, kServiceFragmentShaderId);

  EXPECT_CALL(*gl_, UseProgram(kServiceProgramId))
      .Times(1)
      .RetiresOnSaturation();
  UseProgram cmd;
  cmd.Init(client_program_id_);
  EXPECT_EQ(error::kNoError, ExecuteCmd(cmd));
}
