bool ChildProcessSecurityPolicyImpl::CanReadDirectory(
    int child_id, const FilePath& directory) {
  return HasPermissionsForFile(child_id,
                               directory,
                               kEnumerateDirectoryPermissions);
}
