base::PlatformFileError FileSystemOperation::SetUpFileSystemPath(
    const GURL& path_url,
    FileSystemPath* file_system_path,
    FileSystemFileUtil** file_util,
    SetUpPathMode mode) {
  DCHECK(file_system_path);
  GURL origin_url;
  FileSystemType type;
  FilePath cracked_path;
  if (!CrackFileSystemURL(path_url, &origin_url, &type, &cracked_path))
    return base::PLATFORM_FILE_ERROR_INVALID_URL;

  if (!file_system_context()->GetMountPointProvider(type)->IsAccessAllowed(
          origin_url, type, cracked_path))
    return base::PLATFORM_FILE_ERROR_SECURITY;

  DCHECK(file_util);
  if (!*file_util)
    *file_util = file_system_context()->GetFileUtil(type);
  if (!*file_util)
    return base::PLATFORM_FILE_ERROR_SECURITY;

  file_system_path->set_origin(origin_url);
  file_system_path->set_type(type);
  file_system_path->set_internal_path(cracked_path);

  if (mode == PATH_FOR_READ) {
    FileSystemQuotaUtil* quota_util = file_system_context()->GetQuotaUtil(type);
    if (quota_util) {
      quota_util->NotifyOriginWasAccessedOnIOThread(
          file_system_context()->quota_manager_proxy(),
          file_system_path->origin(), file_system_path->type());
    }
    return base::PLATFORM_FILE_OK;
  }

  DCHECK(mode == PATH_FOR_WRITE || mode == PATH_FOR_CREATE);

  if (cracked_path.value().length() == 0 ||
      cracked_path.DirName().value() == cracked_path.value())
    return base::PLATFORM_FILE_ERROR_SECURITY;

  if (mode == PATH_FOR_CREATE) {
    FileSystemMountPointProvider* provider = file_system_context()->
        GetMountPointProvider(type);

    if (provider->IsRestrictedFileName(VirtualPath::BaseName(cracked_path)))
      return base::PLATFORM_FILE_ERROR_SECURITY;
  }

  return base::PLATFORM_FILE_OK;
}
