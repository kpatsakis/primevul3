void PPB_URLLoader_Impl::SetDefersLoading(bool defers_loading) {
  if (loader_.get()) {
    loader_->setDefersLoading(defers_loading);
    is_asynchronous_load_suspended_ = defers_loading;
  }

}
