void DevToolsWindow::IndexingWorked(int request_id,
                                    const std::string& file_system_path,
                                    int worked) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  base::FundamentalValue request_id_value(request_id);
  StringValue file_system_path_value(file_system_path);
  base::FundamentalValue worked_value(worked);
  CallClientFunction("InspectorFrontendAPI.indexingWorked", &request_id_value,
                     &file_system_path_value, &worked_value);
}
