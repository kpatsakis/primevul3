void GLES2DecoderTestBase::SetupShader(
    GLES2DecoderTestBase::AttribInfo* attribs, size_t num_attribs,
    GLES2DecoderTestBase::UniformInfo* uniforms, size_t num_uniforms,
    GLuint program_client_id, GLuint program_service_id,
    GLuint vertex_shader_client_id, GLuint vertex_shader_service_id,
    GLuint fragment_shader_client_id, GLuint fragment_shader_service_id) {
  {
    InSequence s;

    EXPECT_CALL(*gl_,
                AttachShader(program_service_id, vertex_shader_service_id))
        .Times(1)
        .RetiresOnSaturation();
    EXPECT_CALL(*gl_,
                AttachShader(program_service_id, fragment_shader_service_id))
        .Times(1)
        .RetiresOnSaturation();
    EXPECT_CALL(*gl_, LinkProgram(program_service_id))
        .Times(1)
        .RetiresOnSaturation();
    EXPECT_CALL(*gl_, GetProgramiv(program_service_id, GL_LINK_STATUS, _))
        .WillOnce(SetArgumentPointee<2>(GL_TRUE))
        .RetiresOnSaturation();
    EXPECT_CALL(*gl_,
        GetProgramiv(program_service_id, GL_INFO_LOG_LENGTH, _))
        .WillOnce(SetArgumentPointee<2>(0))
        .RetiresOnSaturation();
    EXPECT_CALL(*gl_,
        GetProgramInfoLog(program_service_id, _, _, _))
        .Times(1)
        .RetiresOnSaturation();
    EXPECT_CALL(*gl_,
        GetProgramiv(program_service_id, GL_ACTIVE_ATTRIBUTES, _))
        .WillOnce(SetArgumentPointee<2>(num_attribs))
        .RetiresOnSaturation();
    size_t max_attrib_len = 0;
    for (size_t ii = 0; ii < num_attribs; ++ii) {
      size_t len = strlen(attribs[ii].name) + 1;
      max_attrib_len = std::max(max_attrib_len, len);
    }
    EXPECT_CALL(*gl_,
        GetProgramiv(program_service_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, _))
        .WillOnce(SetArgumentPointee<2>(max_attrib_len))
        .RetiresOnSaturation();
    for (size_t ii = 0; ii < num_attribs; ++ii) {
      const AttribInfo& info = attribs[ii];
      EXPECT_CALL(*gl_,
          GetActiveAttrib(program_service_id, ii, max_attrib_len, _, _, _, _))
          .WillOnce(DoAll(
              SetArgumentPointee<3>(strlen(info.name)),
              SetArgumentPointee<4>(info.size),
              SetArgumentPointee<5>(info.type),
              SetArrayArgument<6>(info.name,
                                  info.name + strlen(info.name) + 1)))
          .RetiresOnSaturation();
      if (!ProgramManager::IsInvalidPrefix(info.name, strlen(info.name))) {
        EXPECT_CALL(*gl_, GetAttribLocation(program_service_id,
                                            StrEq(info.name)))
            .WillOnce(Return(info.location))
            .RetiresOnSaturation();
      }
    }
    EXPECT_CALL(*gl_,
        GetProgramiv(program_service_id, GL_ACTIVE_UNIFORMS, _))
        .WillOnce(SetArgumentPointee<2>(num_uniforms))
        .RetiresOnSaturation();
    size_t max_uniform_len = 0;
    for (size_t ii = 0; ii < num_uniforms; ++ii) {
      size_t len = strlen(uniforms[ii].name) + 1;
      max_uniform_len = std::max(max_uniform_len, len);
    }
    EXPECT_CALL(*gl_,
        GetProgramiv(program_service_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, _))
        .WillOnce(SetArgumentPointee<2>(max_uniform_len))
        .RetiresOnSaturation();
    for (size_t ii = 0; ii < num_uniforms; ++ii) {
      const UniformInfo& info = uniforms[ii];
      EXPECT_CALL(*gl_,
          GetActiveUniform(program_service_id, ii, max_uniform_len, _, _, _, _))
          .WillOnce(DoAll(
              SetArgumentPointee<3>(strlen(info.name)),
              SetArgumentPointee<4>(info.size),
              SetArgumentPointee<5>(info.type),
              SetArrayArgument<6>(info.name,
                                  info.name + strlen(info.name) + 1)))
          .RetiresOnSaturation();
      if (!ProgramManager::IsInvalidPrefix(info.name, strlen(info.name))) {
        EXPECT_CALL(*gl_, GetUniformLocation(program_service_id,
                                             StrEq(info.name)))
            .WillOnce(Return(info.location))
            .RetiresOnSaturation();
        if (info.size > 1) {
          std::string base_name = info.name;
          size_t array_pos = base_name.rfind("[0]");
          if (base_name.size() > 3 && array_pos == base_name.size() - 3) {
            base_name = base_name.substr(0, base_name.size() - 3);
          }
          for (GLsizei jj = 1; jj < info.size; ++jj) {
            std::string element_name(
                std::string(base_name) + "[" + base::IntToString(jj) + "]");
            EXPECT_CALL(*gl_, GetUniformLocation(program_service_id,
                                                 StrEq(element_name)))
                .WillOnce(Return(info.location + jj * 2))
                .RetiresOnSaturation();
          }
        }
      }
    }
  }

  DoCreateShader(
      GL_VERTEX_SHADER, vertex_shader_client_id, vertex_shader_service_id);
  DoCreateShader(
      GL_FRAGMENT_SHADER, fragment_shader_client_id,
      fragment_shader_service_id);

  GetShaderInfo(vertex_shader_client_id)->SetStatus(true, "");
  GetShaderInfo(fragment_shader_client_id)->SetStatus(true, "");

  AttachShader attach_cmd;
  attach_cmd.Init(program_client_id, vertex_shader_client_id);
  EXPECT_EQ(error::kNoError, ExecuteCmd(attach_cmd));

  attach_cmd.Init(program_client_id, fragment_shader_client_id);
  EXPECT_EQ(error::kNoError, ExecuteCmd(attach_cmd));

  LinkProgram link_cmd;
  link_cmd.Init(program_client_id);

  EXPECT_EQ(error::kNoError, ExecuteCmd(link_cmd));
}
