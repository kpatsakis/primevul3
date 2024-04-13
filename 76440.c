void GLES2DecoderImpl::RestoreStateForNonRenderableTextures() {
  DCHECK(current_program_);
  DCHECK(!current_program_->IsDeleted());
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
            uniform_info->type == GL_SAMPLER_2D ?
                texture_unit.bound_texture_2d :
                texture_unit.bound_texture_cube_map;
        if (!texture_info || !texture_info->CanRender(texture_manager())) {
          glActiveTexture(GL_TEXTURE0 + texture_unit_index);
          texture_info = texture_unit.bind_target == GL_TEXTURE_2D ?
              texture_unit.bound_texture_2d :
              texture_unit.bound_texture_cube_map;
          glBindTexture(texture_unit.bind_target,
                        texture_info ? texture_info->service_id() : 0);
        }
      }
    }
  }
  glActiveTexture(GL_TEXTURE0 + active_texture_unit_);
}
