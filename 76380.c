void GLES2DecoderImpl::DoDrawArrays(
    GLenum mode, GLint first, GLsizei count) {
  if (first < 0) {
    SetGLError(GL_INVALID_ENUM, "glDrawArrays: first < 0");
    return;
  }
  if (IsDrawValid(first + count - 1)) {
    bool simulated_attrib_0 = SimulateAttrib0(first + count - 1);
    bool textures_set = SetBlackTextureForNonRenderableTextures();
    glDrawArrays(mode, first, count);
    if (textures_set) {
      RestoreStateForNonRenderableTextures();
    }
    if (simulated_attrib_0) {
      RestoreStateForSimulatedAttrib0();
    }
  }
}
