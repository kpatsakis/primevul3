content::WebContents* DevToolsWindow::GetInspectedWebContents() {
  return inspected_contents_observer_ ?
      inspected_contents_observer_->web_contents() : NULL;
}
