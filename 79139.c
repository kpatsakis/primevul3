void DevToolsWindow::ActivateWindow() {
  if (IsDocked() && GetInspectedBrowserWindow())
    web_contents_->GetView()->Focus();
  else if (!IsDocked() && !browser_->window()->IsActive())
    browser_->window()->Activate();
}
