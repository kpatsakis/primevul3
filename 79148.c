void DevToolsWindow::CloseWindow() {
  DCHECK(IsDocked());
  web_contents_->GetRenderViewHost()->FirePageBeforeUnload(false);
}
