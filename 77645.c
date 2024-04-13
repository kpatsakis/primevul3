GLES2DecoderImpl::GLES2DecoderImpl(ContextGroup* group)
    : GLES2Decoder(),
      group_(group),
      error_bits_(0),
      pack_alignment_(4),
      unpack_alignment_(4),
      attrib_0_buffer_id_(0),
      attrib_0_buffer_matches_value_(true),
      attrib_0_size_(0),
      fixed_attrib_buffer_id_(0),
      fixed_attrib_buffer_size_(0),
      active_texture_unit_(0),
      clear_red_(0),
      clear_green_(0),
      clear_blue_(0),
      clear_alpha_(0),
      mask_red_(true),
      mask_green_(true),
      mask_blue_(true),
      mask_alpha_(true),
      clear_stencil_(0),
      mask_stencil_front_(-1),
      mask_stencil_back_(-1),
      clear_depth_(1.0f),
      mask_depth_(true),
      enable_scissor_test_(false),
      enable_depth_test_(false),
      enable_stencil_test_(false),
      state_dirty_(true),
      offscreen_target_color_format_(0),
      offscreen_target_depth_format_(0),
      offscreen_target_stencil_format_(0),
      offscreen_target_samples_(0),
      offscreen_target_buffer_preserved_(true),
      offscreen_saved_color_format_(0),
      stream_texture_manager_(NULL),
      back_buffer_color_format_(0),
      back_buffer_has_depth_(false),
      back_buffer_has_stencil_(false),
      teximage2d_faster_than_texsubimage2d_(true),
      bufferdata_faster_than_buffersubdata_(true),
      current_decoder_error_(error::kNoError),
      use_shader_translator_(true),
      validators_(group_->feature_info()->validators()),
      feature_info_(group_->feature_info()),
      tex_image_2d_failed_(false),
      frame_number_(0),
      has_arb_robustness_(false),
      reset_status_(GL_NO_ERROR),
      needs_mac_nvidia_driver_workaround_(false),
      needs_glsl_built_in_function_emulation_(false),
      force_webgl_glsl_validation_(false),
      derivatives_explicitly_enabled_(false),
      compile_shader_always_succeeds_(false) {
  DCHECK(group);

  attrib_0_value_.v[0] = 0.0f;
  attrib_0_value_.v[1] = 0.0f;
  attrib_0_value_.v[2] = 0.0f;
  attrib_0_value_.v[3] = 1.0f;

  if ((gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2 &&
       !feature_info_->feature_flags().chromium_webglsl &&
       !force_webgl_glsl_validation_) ||
      gfx::GetGLImplementation() == gfx::kGLImplementationMockGL ||
      CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableGLSLTranslator)) {
    use_shader_translator_ = false;
  }

  if (IsAngle()) {
    teximage2d_faster_than_texsubimage2d_ = false;
    bufferdata_faster_than_buffersubdata_ = false;
  }
}
