void DevToolsWindow::AddFileSystem() {
  CHECK(web_contents_->GetURL().SchemeIs(chrome::kChromeDevToolsScheme));
  file_helper_->AddFileSystem(
      base::Bind(&DevToolsWindow::FileSystemAdded, weak_factory_.GetWeakPtr()),
      base::Bind(&DevToolsWindow::ShowDevToolsConfirmInfoBar,
                 weak_factory_.GetWeakPtr()));
}
