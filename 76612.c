bool ChildProcessSecurityPolicyImpl::CanReadFileSystem(
    int child_id, const std::string& filesystem_id) {
  return HasPermissionsForFileSystem(child_id,
                                     filesystem_id,
                                     kReadFilePermissions);
}
