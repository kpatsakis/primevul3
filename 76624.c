void ChildProcessSecurityPolicyImpl::GrantReadDirectory(
    int child_id, const FilePath& directory) {
  GrantPermissionsForFile(child_id, directory, kEnumerateDirectoryPermissions);
}
