void ChildProcessSecurityPolicyImpl::GrantReadWriteFileSystem(
    int child_id, const std::string& filesystem_id) {
  GrantPermissionsForFileSystem(child_id, filesystem_id,
                                kReadFilePermissions |
                                kWriteFilePermissions);
}
