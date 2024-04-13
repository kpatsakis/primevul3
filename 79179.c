void DevToolsWindow::IndexingDone(int request_id,
                                  const std::string& file_system_path) {
  indexing_jobs_.erase(request_id);
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  base::FundamentalValue request_id_value(request_id);
  StringValue file_system_path_value(file_system_path);
  CallClientFunction("InspectorFrontendAPI.indexingDone", &request_id_value,
                     &file_system_path_value, NULL);
}
