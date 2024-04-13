bool GLES2DecoderImpl::MakeCurrent() {
  return context_.get() ? context_->MakeCurrent() : false;
}
