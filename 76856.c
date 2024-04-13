void FileSystemOperation::DidGetMetadata(
    const GetMetadataCallback& callback,
    base::PlatformFileError rv,
    const base::PlatformFileInfo& file_info,
    const FilePath& platform_path) {
  callback.Run(rv, file_info, platform_path);
}
