bool GLES2DecoderImpl::Initialize(gfx::GLContext* context,
                                  const gfx::Size& size,
                                  GLES2Decoder* parent,
                                  uint32 parent_client_texture_id) {
  DCHECK(context);
  DCHECK(!context_.get());

  context_.reset(context);

  default_context_.reset(gfx::GLContext::CreateOffscreenGLContext(
      context_.get()));
  if (!default_context_.get()) {
    Destroy();
    return false;
  }

  if (parent)
    parent_ = static_cast<GLES2DecoderImpl*>(parent)->AsWeakPtr();

  if (!MakeCurrent()) {
    Destroy();
    return false;
  }

  CHECK_GL_ERROR();

  if (!group_->Initialize()) {
    Destroy();
    return false;
  }

  vertex_attrib_manager_.Initialize(group_->max_vertex_attribs());

  depth24_stencil8_oes_supported_ =
      context_->HasExtension("GL_OES_packed_depth_stencil");

  glEnableVertexAttribArray(0);
  glGenBuffersARB(1, &attrib_0_buffer_id_);
  glBindBuffer(GL_ARRAY_BUFFER, attrib_0_buffer_id_);
  glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, NULL);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  texture_units_.reset(
      new TextureUnit[group_->max_texture_units()]);
  for (uint32 tt = 0; tt < group_->max_texture_units(); ++tt) {
    texture_units_[tt].bound_texture_2d =
        texture_manager()->GetDefaultTextureInfo(GL_TEXTURE_2D);
    texture_units_[tt].bound_texture_cube_map =
        texture_manager()->GetDefaultTextureInfo(GL_TEXTURE_CUBE_MAP);
  }

  GLuint ids[2];
  glGenTextures(2, ids);
  black_2d_texture_id_ = ids[0];
  black_cube_texture_id_ = ids[1];
  static uint8 black[] = {0, 0, 0, 255};
  glBindTexture(GL_TEXTURE_2D, black_2d_texture_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, black);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, black_cube_texture_id_);
  for (int ii = 0; ii < GLES2Util::kNumFaces; ++ii) {
    glTexImage2D(GLES2Util::IndexToGLFaceTarget(ii), 0, GL_RGBA, 1, 1, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, black);
  }
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  CHECK_GL_ERROR();

  if (context_->IsOffscreen()) {
    offscreen_target_frame_buffer_.reset(new FrameBuffer(this));
    offscreen_target_frame_buffer_->Create();
    offscreen_target_color_texture_.reset(new Texture(this));
    offscreen_target_color_texture_->Create();
    offscreen_target_depth_render_buffer_.reset(
        new RenderBuffer(this));
    offscreen_target_depth_render_buffer_->Create();
    offscreen_target_stencil_render_buffer_.reset(
        new RenderBuffer(this));
    offscreen_target_stencil_render_buffer_->Create();

    offscreen_saved_color_texture_.reset(new Texture(this));
    offscreen_saved_color_texture_->Create();

    if (parent_) {
      GLuint service_id = offscreen_saved_color_texture_->id();
      TextureManager::TextureInfo* info =
          parent_->CreateTextureInfo(parent_client_texture_id, service_id);
      parent_->texture_manager()->SetInfoTarget(info, GL_TEXTURE_2D);
    }

    pending_offscreen_size_ = size;
    if (!UpdateOffscreenFrameBufferSize()) {
      DLOG(ERROR) << "Could not allocate offscreen buffer storage.";
      Destroy();
      return false;
    }

    DoBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  if (gfx::GetGLImplementation() != gfx::kGLImplementationEGLGLES2) {
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  }

#if defined(GLES2_GPU_SERVICE_TRANSLATE_SHADER)
  if (!ShInitialize()) {
    DLOG(ERROR) << "Could not initialize GLSL translator.";
    Destroy();
    return false;
  }

  TBuiltInResource resources;
  resources.maxVertexAttribs = group_->max_vertex_attribs();
  resources.maxVertexUniformVectors =
      group_->max_vertex_uniform_vectors();
  resources.maxVaryingVectors = group_->max_varying_vectors();
  resources.maxVertexTextureImageUnits =
      group_->max_vertex_texture_image_units();
  resources.maxCombinedTextureImageUnits = group_->max_texture_units();
  resources.maxTextureImageUnits = group_->max_texture_image_units();
  resources.maxFragmentUniformVectors =
      group_->max_fragment_uniform_vectors();
  resources.maxDrawBuffers = 1;
  vertex_compiler_ = ShConstructCompiler(EShLangVertex, EShSpecGLES2,
      &resources);
  if (vertex_compiler_ == NULL) {
      DLOG(ERROR) << "Could not create vertex shader translator.";
      Destroy();
      return false;
  }
  fragment_compiler_ = ShConstructCompiler(EShLangFragment, EShSpecGLES2,
      &resources);
  if (fragment_compiler_ == NULL) {
      DLOG(ERROR) << "Could not create fragment shader translator.";
      Destroy();
      return false;
  }

#endif  // GLES2_GPU_SERVICE_TRANSLATE_SHADER

  return true;
}
