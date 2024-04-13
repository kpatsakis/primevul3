void DevToolsWindow::SearchCompleted(
    int request_id,
    const std::string& file_system_path,
    const std::vector<std::string>& file_paths) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  ListValue file_paths_value;
  for (std::vector<std::string>::const_iterator it(file_paths.begin());
       it != file_paths.end(); ++it) {
    file_paths_value.AppendString(*it);
  }
  base::FundamentalValue request_id_value(request_id);
  StringValue file_system_path_value(file_system_path);
  CallClientFunction("InspectorFrontendAPI.searchCompleted", &request_id_value,
                     &file_system_path_value, &file_paths_value);
}
