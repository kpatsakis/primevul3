void FileSystemOperation::DidDirectoryExists(
    const StatusCallback& callback,
    base::PlatformFileError rv,
    const base::PlatformFileInfo& file_info,
    const FilePath& unused) {
  if (rv == base::PLATFORM_FILE_OK && !file_info.is_directory)
    rv = base::PLATFORM_FILE_ERROR_NOT_A_DIRECTORY;
  callback.Run(rv);
}
