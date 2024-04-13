void GetMetadataForSnapshot(
    const FileSystemOperationInterface::SnapshotFileCallback& callback,
    base::PlatformFileError result,
    const base::PlatformFileInfo& file_info,
    const FilePath& platform_path) {
  callback.Run(result, file_info, platform_path, NULL);
}
