void DevToolsWindow::IndexingTotalWorkCalculated(
    int request_id,
    const std::string& file_system_path,
    int total_work) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  base::FundamentalValue request_id_value(request_id);
  StringValue file_system_path_value(file_system_path);
  base::FundamentalValue total_work_value(total_work);
  CallClientFunction("InspectorFrontendAPI.indexingTotalWorkCalculated",
                     &request_id_value, &file_system_path_value,
                     &total_work_value);
}
