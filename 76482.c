void GLES2DecoderTestBase::InitDecoder(const char* extensions) {
  gl_.reset(new StrictMock<MockGLInterface>());
  ::gfx::GLInterface::SetGLInterface(gl_.get());

  InSequence sequence;

  EXPECT_CALL(*gl_, GetString(GL_EXTENSIONS))
      .WillOnce(Return(reinterpret_cast<const uint8*>(extensions)))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, GetIntegerv(GL_MAX_VERTEX_ATTRIBS, _))
      .WillOnce(SetArgumentPointee<1>(kNumVertexAttribs))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, GetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, _))
      .WillOnce(SetArgumentPointee<1>(kNumTextureUnits))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, GetIntegerv(GL_MAX_TEXTURE_SIZE, _))
      .WillOnce(SetArgumentPointee<1>(kMaxTextureSize))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, GetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, _))
      .WillOnce(SetArgumentPointee<1>(kMaxCubeMapTextureSize))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, GetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, _))
      .WillOnce(SetArgumentPointee<1>(kMaxTextureImageUnits))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, GetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, _))
      .WillOnce(SetArgumentPointee<1>(kMaxVertexTextureImageUnits))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, GetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, _))
      .WillOnce(SetArgumentPointee<1>(kMaxFragmentUniformVectors))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, GetIntegerv(GL_MAX_VARYING_FLOATS, _))
      .WillOnce(SetArgumentPointee<1>(kMaxVaryingVectors))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, GetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, _))
      .WillOnce(SetArgumentPointee<1>(kMaxVertexUniformVectors))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, EnableVertexAttribArray(0))
      .Times(1)
      .RetiresOnSaturation();
  static GLuint attrib_ids[] = {
    kServiceAttrib0BufferId,
  };
  EXPECT_CALL(*gl_, GenBuffersARB(arraysize(attrib_ids), _))
      .WillOnce(SetArrayArgument<1>(attrib_ids,
                                    attrib_ids + arraysize(attrib_ids)))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, BindBuffer(GL_ARRAY_BUFFER, kServiceAttrib0BufferId))
      .Times(1)
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, VertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, NULL))
      .Times(1)
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, BindBuffer(GL_ARRAY_BUFFER, 0))
      .Times(1)
      .RetiresOnSaturation();

  static GLuint black_ids[] = {
    kServiceBlackTexture2dId,
    kServiceBlackTextureCubemapId,
  };
  EXPECT_CALL(*gl_, GenTextures(arraysize(black_ids), _))
      .WillOnce(SetArrayArgument<1>(black_ids,
                                    black_ids + arraysize(black_ids)))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, BindTexture(GL_TEXTURE_2D, kServiceBlackTexture2dId))
      .Times(1)
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
                               GL_UNSIGNED_BYTE, _))
      .Times(1)
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1)
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, BindTexture(GL_TEXTURE_CUBE_MAP,
                                kServiceBlackTextureCubemapId))
      .Times(1)
      .RetiresOnSaturation();
  static GLenum faces[] = {
      GL_TEXTURE_CUBE_MAP_POSITIVE_X,
      GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
      GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
      GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
      GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
      GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
  };
  for (size_t ii = 0; ii < arraysize(faces); ++ii) {
    EXPECT_CALL(*gl_, TexImage2D(faces[ii], 0, GL_RGBA, 1, 1, 0, GL_RGBA,
                                 GL_UNSIGNED_BYTE, _))
        .Times(1)
        .RetiresOnSaturation();
  }
  EXPECT_CALL(*gl_, BindTexture(GL_TEXTURE_CUBE_MAP, 0))
      .Times(1)
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, Enable(GL_VERTEX_PROGRAM_POINT_SIZE))
      .Times(1)
      .RetiresOnSaturation();

  engine_.reset(new StrictMock<MockCommandBufferEngine>());
  Buffer buffer = engine_->GetSharedMemoryBuffer(kSharedMemoryId);
  shared_memory_offset_ = kSharedMemoryOffset;
  shared_memory_address_ = reinterpret_cast<int8*>(buffer.ptr) +
      shared_memory_offset_;
  shared_memory_id_ = kSharedMemoryId;

  context_ = new gfx::StubGLContext;

  decoder_.reset(GLES2Decoder::Create(&group_));
  decoder_->Initialize(context_, gfx::Size(), NULL, 0);
  decoder_->set_engine(engine_.get());

  EXPECT_CALL(*gl_, GenBuffersARB(_, _))
      .WillOnce(SetArgumentPointee<1>(kServiceBufferId))
      .RetiresOnSaturation();
  GenHelper<GenBuffersImmediate>(client_buffer_id_);
  EXPECT_CALL(*gl_, GenFramebuffersEXT(_, _))
      .WillOnce(SetArgumentPointee<1>(kServiceFramebufferId))
      .RetiresOnSaturation();
  GenHelper<GenFramebuffersImmediate>(client_framebuffer_id_);
  EXPECT_CALL(*gl_, GenRenderbuffersEXT(_, _))
      .WillOnce(SetArgumentPointee<1>(kServiceRenderbufferId))
      .RetiresOnSaturation();
  GenHelper<GenRenderbuffersImmediate>(client_renderbuffer_id_);
  EXPECT_CALL(*gl_, GenTextures(_, _))
      .WillOnce(SetArgumentPointee<1>(kServiceTextureId))
      .RetiresOnSaturation();
  GenHelper<GenTexturesImmediate>(client_texture_id_);
  EXPECT_CALL(*gl_, GenBuffersARB(_, _))
      .WillOnce(SetArgumentPointee<1>(kServiceElementBufferId))
      .RetiresOnSaturation();
  GenHelper<GenBuffersImmediate>(client_element_buffer_id_);

  {
    EXPECT_CALL(*gl_, CreateProgram())
        .Times(1)
        .WillOnce(Return(kServiceProgramId))
        .RetiresOnSaturation();
    CreateProgram cmd;
    cmd.Init(client_program_id_);
    EXPECT_EQ(error::kNoError, ExecuteCmd(cmd));
  }

  DoCreateShader(GL_VERTEX_SHADER, client_shader_id_, kServiceShaderId);

  EXPECT_EQ(GL_NO_ERROR, GetGLError());
}
