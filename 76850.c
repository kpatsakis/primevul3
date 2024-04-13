void FileSystemOperation::CreateSnapshotFile(
    const GURL& path_url,
    const SnapshotFileCallback& callback) {
  GetMetadata(path_url, base::Bind(&GetMetadataForSnapshot, callback));
}
