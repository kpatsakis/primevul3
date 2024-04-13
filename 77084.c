void DesktopNativeWidgetHelperAura::OnRootWindowHostClosed(
    const aura::RootWindow* root) {
  DCHECK_EQ(root, root_window_.get());
  widget_->GetWidget()->Close();
}
