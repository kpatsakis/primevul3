bool ChildProcessSecurityPolicyImpl::CanReadFile(int child_id,
                                                 const FilePath& file) {
  return HasPermissionsForFile(child_id, file, kReadFilePermissions);
}
