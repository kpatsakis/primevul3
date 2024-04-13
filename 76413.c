  TextureManager::TextureInfo* GetTextureInfoForTarget(GLenum target) {
    TextureUnit& unit = texture_units_[active_texture_unit_];
    TextureManager::TextureInfo* info = NULL;
    switch (target) {
      case GL_TEXTURE_2D:
        info = unit.bound_texture_2d;
        break;
      case GL_TEXTURE_CUBE_MAP:
      case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
      case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
      case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
      case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
      case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
      case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
        info = unit.bound_texture_cube_map;
        break;
      default:
        NOTREACHED();
        return NULL;
    }
    return (info && !info->IsDeleted()) ? info : NULL;
  }
