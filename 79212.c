void DevToolsWindow::StopIndexing(int request_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  IndexingJobsMap::iterator it = indexing_jobs_.find(request_id);
  if (it == indexing_jobs_.end())
    return;
  it->second->Stop();
  indexing_jobs_.erase(it);
}
