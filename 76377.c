void GLES2DecoderImpl::DoDepthMask(GLboolean depth) {
  mask_depth_ = depth;
  glDepthMask(depth);
}
