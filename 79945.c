bool VerifyPathControlledByAdmin(const FilePath& path) {
  const unsigned kRootUid = 0;
  const FilePath kFileSystemRoot("/");

  const char* const kAdminGroupNames[] = {
    "admin",
    "wheel"
  };

  base::ThreadRestrictions::AssertIOAllowed();

  std::set<gid_t> allowed_group_ids;
  for (int i = 0, ie = arraysize(kAdminGroupNames); i < ie; ++i) {
    struct group *group_record = getgrnam(kAdminGroupNames[i]);
    if (!group_record) {
      DPLOG(ERROR) << "Could not get the group ID of group \""
                   << kAdminGroupNames[i] << "\".";
      continue;
    }

    allowed_group_ids.insert(group_record->gr_gid);
  }

  return VerifyPathControlledByUser(
      kFileSystemRoot, path, kRootUid, allowed_group_ids);
}
