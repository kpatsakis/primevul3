views::View* OmniboxViewWin::AddToView(views::View* parent) {
  native_view_host_ = new OmniboxViewWrapper(this);
  parent->AddChildView(native_view_host_);
  native_view_host_->set_focus_view(parent);
  native_view_host_->Attach(GetNativeView());
  return native_view_host_;
}
