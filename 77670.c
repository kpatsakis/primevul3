bool GLES2DecoderImpl::Initialize(
    const scoped_refptr<gfx::GLSurface>& surface,
    const scoped_refptr<gfx::GLContext>& context,
    bool offscreen,
    const gfx::Size& size,
    const DisallowedFeatures& disallowed_features,
    const char* allowed_extensions,
    const std::vector<int32>& attribs) {
  DCHECK(context);
  DCHECK(!context_.get());

  if (CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableGPUDebugging)) {
    set_debug(true);
  }

  if (CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableGPUCommandLogging)) {
    set_log_commands(true);
  }

  compile_shader_always_succeeds_ = CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kCompileShaderAlwaysSucceeds);

  surface_ = surface;

  context_ = context;

  if (!MakeCurrent()) {
    LOG(ERROR) << "GLES2DecoderImpl::Initialize failed because "
               << "MakeCurrent failed.";
    group_ = NULL;  // Must not destroy ContextGroup if it is not initialized.
    Destroy();
    return false;
  }

  if (!group_->Initialize(disallowed_features, allowed_extensions)) {
    LOG(ERROR) << "GpuScheduler::InitializeCommon failed because group "
               << "failed to initialize.";
    group_ = NULL;  // Must not destroy ContextGroup if it is not initialized.
    Destroy();
    return false;
  }

  CHECK_GL_ERROR();
  disallowed_features_ = disallowed_features;

  vertex_attrib_manager_.reset(new VertexAttribManager());
  vertex_attrib_manager_->Initialize(group_->max_vertex_attribs());

  query_manager_.reset(new QueryManager(this, feature_info_->feature_flags(
      ).use_arb_occlusion_query2_for_occlusion_query_boolean));

  util_.set_num_compressed_texture_formats(
      validators_->compressed_texture_format.GetValues().size());

  if (gfx::GetGLImplementation() != gfx::kGLImplementationEGLGLES2) {
    glEnableVertexAttribArray(0);
  }
  glGenBuffersARB(1, &attrib_0_buffer_id_);
  glBindBuffer(GL_ARRAY_BUFFER, attrib_0_buffer_id_);
  glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, NULL);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glGenBuffersARB(1, &fixed_attrib_buffer_id_);

  texture_units_.reset(
      new TextureUnit[group_->max_texture_units()]);
  for (uint32 tt = 0; tt < group_->max_texture_units(); ++tt) {
    glActiveTexture(GL_TEXTURE0 + tt);
    TextureManager::TextureInfo* info;
    if (feature_info_->feature_flags().oes_egl_image_external) {
      info = texture_manager()->GetDefaultTextureInfo(GL_TEXTURE_EXTERNAL_OES);
      texture_units_[tt].bound_texture_external_oes = info;
      glBindTexture(GL_TEXTURE_EXTERNAL_OES, info->service_id());
    }
    if (feature_info_->feature_flags().arb_texture_rectangle) {
      info = texture_manager()->GetDefaultTextureInfo(GL_TEXTURE_RECTANGLE_ARB);
      texture_units_[tt].bound_texture_rectangle_arb = info;
      glBindTexture(GL_TEXTURE_RECTANGLE_ARB, info->service_id());
    }
    info = texture_manager()->GetDefaultTextureInfo(GL_TEXTURE_CUBE_MAP);
    texture_units_[tt].bound_texture_cube_map = info;
    glBindTexture(GL_TEXTURE_CUBE_MAP, info->service_id());
    info = texture_manager()->GetDefaultTextureInfo(GL_TEXTURE_2D);
    texture_units_[tt].bound_texture_2d = info;
    glBindTexture(GL_TEXTURE_2D, info->service_id());
  }
  glActiveTexture(GL_TEXTURE0);
  CHECK_GL_ERROR();

  ContextCreationAttribParser attrib_parser;
  if (!attrib_parser.Parse(attribs))
    return false;


  GLint v = 0;
  glGetIntegerv(GL_ALPHA_BITS, &v);
  back_buffer_color_format_ =
      (attrib_parser.alpha_size_ != 0 && v > 0) ? GL_RGBA : GL_RGB;
  glGetIntegerv(GL_DEPTH_BITS, &v);
  back_buffer_has_depth_ = attrib_parser.depth_size_ != 0 && v > 0;
  glGetIntegerv(GL_STENCIL_BITS, &v);
  back_buffer_has_stencil_ = attrib_parser.stencil_size_ != 0 && v > 0;

  if (offscreen) {
    if (attrib_parser.samples_ > 0 && attrib_parser.sample_buffers_ > 0 &&
        (context_->HasExtension("GL_EXT_framebuffer_multisample") ||
         context_->HasExtension("GL_ANGLE_framebuffer_multisample"))) {
      GLint max_sample_count = 1;
      glGetIntegerv(GL_MAX_SAMPLES_EXT, &max_sample_count);
      offscreen_target_samples_ = std::min(attrib_parser.samples_,
                                           max_sample_count);
    } else {
      offscreen_target_samples_ = 1;
    }
    offscreen_target_buffer_preserved_ = attrib_parser.buffer_preserved_;

    if (gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2) {
      const bool rgb8_supported =
          context_->HasExtension("GL_OES_rgb8_rgba8");
      if (rgb8_supported && offscreen_target_samples_ > 1) {
        offscreen_target_color_format_ = attrib_parser.alpha_size_ > 0 ?
            GL_RGBA8 : GL_RGB8;
      } else {
        offscreen_target_samples_ = 1;
        offscreen_target_color_format_ = attrib_parser.alpha_size_ > 0 ?
            GL_RGBA : GL_RGB;
      }

      const bool depth24_stencil8_supported =
          context_->HasExtension("GL_OES_packed_depth_stencil");
      VLOG(1) << "GL_OES_packed_depth_stencil "
              << (depth24_stencil8_supported ? "" : "not ") << "supported.";
      if ((attrib_parser.depth_size_ > 0 || attrib_parser.stencil_size_ > 0) &&
          depth24_stencil8_supported) {
        offscreen_target_depth_format_ = GL_DEPTH24_STENCIL8;
        offscreen_target_stencil_format_ = 0;
      } else {
        offscreen_target_depth_format_ = attrib_parser.depth_size_ > 0 ?
            GL_DEPTH_COMPONENT16 : 0;
        offscreen_target_stencil_format_ = attrib_parser.stencil_size_ > 0 ?
            GL_STENCIL_INDEX8 : 0;
      }
    } else {
      offscreen_target_color_format_ = attrib_parser.alpha_size_ > 0 ?
          GL_RGBA : GL_RGB;

      const bool depth24_stencil8_supported =
          context_->HasExtension("GL_EXT_packed_depth_stencil");
      VLOG(1) << "GL_EXT_packed_depth_stencil "
              << (depth24_stencil8_supported ? "" : "not ") << "supported.";

      if ((attrib_parser.depth_size_ > 0 || attrib_parser.stencil_size_ > 0) &&
          depth24_stencil8_supported) {
        offscreen_target_depth_format_ = GL_DEPTH24_STENCIL8;
        offscreen_target_stencil_format_ = 0;
      } else {
        offscreen_target_depth_format_ = attrib_parser.depth_size_ > 0 ?
            GL_DEPTH_COMPONENT : 0;
        offscreen_target_stencil_format_ = attrib_parser.stencil_size_ > 0 ?
            GL_STENCIL_INDEX : 0;
      }
    }

    offscreen_saved_color_format_ = attrib_parser.alpha_size_ > 0 ?
        GL_RGBA : GL_RGB;

    offscreen_target_frame_buffer_.reset(new FrameBuffer(this));
    offscreen_target_frame_buffer_->Create();
    if (IsOffscreenBufferMultisampled()) {
      offscreen_target_color_render_buffer_.reset(new RenderBuffer(this));
      offscreen_target_color_render_buffer_->Create();
    } else {
      offscreen_target_color_texture_.reset(new Texture(this));
      offscreen_target_color_texture_->Create();
    }
    offscreen_target_depth_render_buffer_.reset(new RenderBuffer(this));
    offscreen_target_depth_render_buffer_->Create();
    offscreen_target_stencil_render_buffer_.reset(new RenderBuffer(this));
    offscreen_target_stencil_render_buffer_->Create();

    offscreen_saved_frame_buffer_.reset(new FrameBuffer(this));
    offscreen_saved_frame_buffer_->Create();
    offscreen_saved_color_texture_.reset(new Texture(this));
    offscreen_saved_color_texture_->Create();

    if (!ResizeOffscreenFrameBuffer(size)) {
      LOG(ERROR) << "Could not allocate offscreen buffer storage.";
      Destroy();
      return false;
    }

    DoBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  if (gfx::GetGLImplementation() != gfx::kGLImplementationEGLGLES2) {
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SPRITE);
  }

  has_arb_robustness_ = context->HasExtension("GL_ARB_robustness");

  if (!disallowed_features_.driver_bug_workarounds) {
#if defined(OS_MACOSX)
    const char* vendor_str = reinterpret_cast<const char*>(
        glGetString(GL_VENDOR));
    needs_mac_nvidia_driver_workaround_ =
        vendor_str && strstr(vendor_str, "NVIDIA");
    needs_glsl_built_in_function_emulation_ =
        vendor_str && (strstr(vendor_str, "ATI") || strstr(vendor_str, "AMD"));
#elif defined(OS_WIN)
    if (gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2)
      needs_glsl_built_in_function_emulation_ = true;
#endif
  }

  if (!InitializeShaderTranslator()) {
    return false;
  }

  return true;
}
