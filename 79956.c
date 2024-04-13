bool SetReparsePoint(HANDLE source, const FilePath& target_path) {
  std::wstring kPathPrefix = L"\\??\\";
  std::wstring target_str;
  if (kPathPrefix != target_path.value().substr(0, kPathPrefix.size()))
    target_str += kPathPrefix;
  target_str += target_path.value();
  const wchar_t* target = target_str.c_str();
  USHORT size_target = static_cast<USHORT>(wcslen(target)) * sizeof(target[0]);
  char buffer[2000] = {0};
  DWORD returned;

  REPARSE_DATA_BUFFER* data = reinterpret_cast<REPARSE_DATA_BUFFER*>(buffer);

  data->ReparseTag = 0xa0000003;
  memcpy(data->MountPointReparseBuffer.PathBuffer, target, size_target + 2);

  data->MountPointReparseBuffer.SubstituteNameLength = size_target;
  data->MountPointReparseBuffer.PrintNameOffset = size_target + 2;
  data->ReparseDataLength = size_target + 4 + 8;

  int data_size = data->ReparseDataLength + 8;

  if (!DeviceIoControl(source, FSCTL_SET_REPARSE_POINT, &buffer, data_size,
                       NULL, 0, &returned, NULL)) {
    return false;
  }
  return true;
}
