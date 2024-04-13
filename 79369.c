FileError AddMyDriveIfNeeded(
    ResourceMetadata* resource_metadata,
    const google_apis::AboutResource& about_resource,
    base::FilePath* changed_directory_path) {
  ResourceEntry entry;
  FileError error = resource_metadata->GetResourceEntryByPath(
      util::GetDriveMyDriveRootPath(), &entry);
  if (error != FILE_ERROR_NOT_FOUND)
    return error;

  std::string local_id;
  error = resource_metadata->AddEntry(
      util::CreateMyDriveRootEntry(about_resource.root_folder_id()),
      &local_id);
  if (error != FILE_ERROR_OK)
    return error;

  *changed_directory_path = util::GetDriveGrandRootPath();
  return FILE_ERROR_OK;
}
