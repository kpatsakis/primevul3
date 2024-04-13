void DevToolsWindow::FileSystemAdded(
    const DevToolsFileHelper::FileSystem& file_system) {
  scoped_ptr<base::StringValue> error_string_value(
      new base::StringValue(std::string()));
  scoped_ptr<base::DictionaryValue> file_system_value;
  if (!file_system.file_system_path.empty())
    file_system_value.reset(CreateFileSystemValue(file_system));
  CallClientFunction("InspectorFrontendAPI.fileSystemAdded",
                     error_string_value.get(), file_system_value.get(), NULL);
}
