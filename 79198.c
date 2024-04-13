void DevToolsWindow::RequestFileSystems() {
  CHECK(web_contents_->GetURL().SchemeIs(chrome::kChromeDevToolsScheme));
  file_helper_->RequestFileSystems(base::Bind(
      &DevToolsWindow::FileSystemsLoaded, weak_factory_.GetWeakPtr()));
}
