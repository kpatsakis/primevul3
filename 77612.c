void GLES2DecoderImpl::DoClear(GLbitfield mask) {
  if (CheckBoundFramebuffersValid("glClear")) {
    UNSHIPPED_TRACE_EVENT_INSTANT2("test_gpu", "DoClear", "red", clear_red_,
                                   "green", clear_green_);
    ApplyDirtyState();
    glClear(mask);
  }
}
