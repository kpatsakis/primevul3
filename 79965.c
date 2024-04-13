bool RenderViewImpl::CanComposeInline() {
  return pepper_helper_->IsPluginFocused() ?
      pepper_helper_->CanComposeInline() : true;
}
