void ChildProcessSecurityPolicyImpl::GrantReadFile(int child_id,
                                                   const FilePath& file) {
  GrantPermissionsForFile(child_id, file, kReadFilePermissions);
}
