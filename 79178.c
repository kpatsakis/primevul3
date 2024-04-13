void DevToolsWindow::IndexPath(int request_id,
                               const std::string& file_system_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  CHECK(web_contents_->GetURL().SchemeIs(chrome::kChromeDevToolsScheme));
  if (!file_helper_->IsFileSystemAdded(file_system_path)) {
    IndexingDone(request_id, file_system_path);
    return;
  }
  indexing_jobs_[request_id] =
      scoped_refptr<DevToolsFileSystemIndexer::FileSystemIndexingJob>(
          file_system_indexer_->IndexPath(
              file_system_path,
              Bind(&DevToolsWindow::IndexingTotalWorkCalculated,
                   weak_factory_.GetWeakPtr(),
                   request_id,
                   file_system_path),
              Bind(&DevToolsWindow::IndexingWorked,
                   weak_factory_.GetWeakPtr(),
                   request_id,
                   file_system_path),
              Bind(&DevToolsWindow::IndexingDone,
                   weak_factory_.GetWeakPtr(),
                   request_id,
                   file_system_path)));
}
