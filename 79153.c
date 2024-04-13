DictionaryValue* CreateFileSystemValue(
    DevToolsFileHelper::FileSystem file_system) {
  DictionaryValue* file_system_value = new DictionaryValue();
  file_system_value->SetString("fileSystemName", file_system.file_system_name);
  file_system_value->SetString("rootURL", file_system.root_url);
  file_system_value->SetString("fileSystemPath", file_system.file_system_path);
  return file_system_value;
}
