bool GLES2DecoderImpl::SetBlackTextureForNonRenderableTextures() {
  DCHECK(current_program_);
  if (!texture_manager()->HaveUnrenderableTextures()) {
    return false;
  }
  bool textures_set = false;
  const ProgramManager::ProgramInfo::SamplerIndices& sampler_indices =
     current_program_->sampler_indices();
  for (size_t ii = 0; ii < sampler_indices.size(); ++ii) {
    const ProgramManager::ProgramInfo::UniformInfo* uniform_info =
        current_program_->GetUniformInfo(sampler_indices[ii]);
    DCHECK(uniform_info);
    for (size_t jj = 0; jj < uniform_info->texture_units.size(); ++jj) {
      GLuint texture_unit_index = uniform_info->texture_units[jj];
      if (texture_unit_index < group_->max_texture_units()) {
        TextureUnit& texture_unit = texture_units_[texture_unit_index];
        TextureManager::TextureInfo* texture_info =
            texture_unit.GetInfoForSamplerType(uniform_info->type);
        if (!texture_info || !texture_manager()->CanRender(texture_info)) {
          textures_set = true;
          glActiveTexture(GL_TEXTURE0 + texture_unit_index);
          glBindTexture(
              GetBindTargetForSamplerType(uniform_info->type),
              texture_manager()->black_texture_id(uniform_info->type));
        }
      }
    }
  }
  return textures_set;
}
