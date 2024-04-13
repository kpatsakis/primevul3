void DevToolsWindow::SearchInPath(int request_id,
                                  const std::string& file_system_path,
                                  const std::string& query) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  CHECK(web_contents_->GetURL().SchemeIs(chrome::kChromeDevToolsScheme));
  if (!file_helper_->IsFileSystemAdded(file_system_path)) {
    SearchCompleted(request_id, file_system_path, std::vector<std::string>());
    return;
  }
  file_system_indexer_->SearchInPath(file_system_path,
                                     query,
                                     Bind(&DevToolsWindow::SearchCompleted,
                                          weak_factory_.GetWeakPtr(),
                                          request_id,
                                          file_system_path));
}
