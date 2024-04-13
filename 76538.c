void PPB_URLLoader_Impl::RunCallback(int32_t result) {
  if (!pending_callback_.get()) {
    CHECK(main_document_loader_);
    return;
  }

  DCHECK(!user_buffer_);

  user_buffer_ = NULL;
  user_buffer_size_ = 0;
  pending_callback_->Run(result);
}
