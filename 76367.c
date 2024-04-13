void GLES2DecoderImpl::DoBindTexture(GLenum target, GLuint client_id) {
  TextureManager::TextureInfo* info = NULL;
  GLuint service_id = 0;
  if (client_id != 0) {
    info = GetTextureInfo(client_id);
    if (!info) {
      glGenTextures(1, &service_id);
      CreateTextureInfo(client_id, service_id);
      info = GetTextureInfo(client_id);
      IdAllocator* id_allocator =
          group_->GetIdAllocator(id_namespaces::kTextures);
      id_allocator->MarkAsUsed(client_id);
    }
  } else {
    info = texture_manager()->GetDefaultTextureInfo(target);
  }

  if (info->target() != 0 && info->target() != target) {
    SetGLError(GL_INVALID_OPERATION,
               "glBindTexture: texture bound to more than 1 target.");
    return;
  }
  if (info->target() == 0) {
    texture_manager()->SetInfoTarget(info, target);
  }
  glBindTexture(target, info->service_id());
  TextureUnit& unit = texture_units_[active_texture_unit_];
  unit.bind_target = target;
  switch (target) {
    case GL_TEXTURE_2D:
      unit.bound_texture_2d = info;
      break;
    case GL_TEXTURE_CUBE_MAP:
      unit.bound_texture_cube_map = info;
      break;
    default:
      NOTREACHED();  // Validation should prevent us getting here.
      break;
  }
}
