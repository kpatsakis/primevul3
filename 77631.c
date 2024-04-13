void GLES2DecoderImpl::DoTexImageIOSurface2DCHROMIUM(
    GLenum target, GLsizei width, GLsizei height,
    GLuint io_surface_id, GLuint plane) {
#if defined(OS_MACOSX)
  if (gfx::GetGLImplementation() != gfx::kGLImplementationDesktopGL) {
    SetGLError(GL_INVALID_OPERATION,
               "glTexImageIOSurface2DCHROMIUM: only supported on desktop GL.");
    return;
  }

  IOSurfaceSupport* surface_support = IOSurfaceSupport::Initialize();
  if (!surface_support) {
    SetGLError(GL_INVALID_OPERATION,
               "glTexImageIOSurface2DCHROMIUM: only supported on 10.6.");
    return;
  }

  if (target != GL_TEXTURE_RECTANGLE_ARB) {
    SetGLError(
        GL_INVALID_OPERATION,
        "glTexImageIOSurface2DCHROMIUM: requires TEXTURE_RECTANGLE_ARB target");
    return;
  }

  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_OPERATION,
               "glTexImageIOSurface2DCHROMIUM: no rectangle texture bound");
    return;
  }
  if (info == texture_manager()->GetDefaultTextureInfo(target)) {
    SetGLError(GL_INVALID_OPERATION,
               "glTexImageIOSurface2DCHROMIUM: can't bind default texture");
    return;
  }

  CFTypeRef surface = surface_support->IOSurfaceLookup(io_surface_id);
  if (!surface) {
    SetGLError(GL_INVALID_OPERATION,
               "glTexImageIOSurface2DCHROMIUM: no IOSurface with the given ID");
    return;
  }

  ReleaseIOSurfaceForTexture(info->service_id());

  texture_to_io_surface_map_.insert(
      std::make_pair(info->service_id(), surface));

  CGLContextObj context =
      static_cast<CGLContextObj>(context_->GetHandle());

  CGLError err = surface_support->CGLTexImageIOSurface2D(
      context,
      target,
      GL_RGBA,
      width,
      height,
      GL_BGRA,
      GL_UNSIGNED_INT_8_8_8_8_REV,
      surface,
      plane);

  if (err != kCGLNoError) {
    SetGLError(
        GL_INVALID_OPERATION,
        "glTexImageIOSurface2DCHROMIUM: error in CGLTexImageIOSurface2D");
    return;
  }

  texture_manager()->SetLevelInfo(
      info, target, 0, GL_RGBA, width, height, 1, 0,
      GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, true);

#else
  SetGLError(GL_INVALID_OPERATION,
             "glTexImageIOSurface2DCHROMIUM: not supported.");
#endif
}
