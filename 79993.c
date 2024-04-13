ui::TextInputType RenderViewImpl::GetTextInputType() {
  return pepper_helper_->IsPluginFocused() ?
      pepper_helper_->GetTextInputType() : RenderWidget::GetTextInputType();
}
