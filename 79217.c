void DevToolsWindow::UpgradeDraggedFileSystemPermissions(
    const std::string& file_system_url) {
  CHECK(web_contents_->GetURL().SchemeIs(chrome::kChromeDevToolsScheme));
  file_helper_->UpgradeDraggedFileSystemPermissions(
      file_system_url,
      base::Bind(&DevToolsWindow::FileSystemAdded, weak_factory_.GetWeakPtr()),
      base::Bind(&DevToolsWindow::ShowDevToolsConfirmInfoBar,
                 weak_factory_.GetWeakPtr()));
}
