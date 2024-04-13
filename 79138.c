void DevToolsWindow::ActivateContents(content::WebContents* contents) {
  if (IsDocked()) {
    content::WebContents* inspected_tab = this->GetInspectedWebContents();
    inspected_tab->GetDelegate()->ActivateContents(inspected_tab);
  } else {
    browser_->window()->Activate();
  }
}
