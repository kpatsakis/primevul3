bool VerifySpecificPathControlledByUser(const FilePath& path,
                                        uid_t owner_uid,
                                        const std::set<gid_t>& group_gids) {
  stat_wrapper_t stat_info;
  if (CallLstat(path.value().c_str(), &stat_info) != 0) {
    DPLOG(ERROR) << "Failed to get information on path "
                 << path.value();
    return false;
  }

  if (S_ISLNK(stat_info.st_mode)) {
    DLOG(ERROR) << "Path " << path.value()
               << " is a symbolic link.";
    return false;
  }

  if (stat_info.st_uid != owner_uid) {
    DLOG(ERROR) << "Path " << path.value()
                << " is owned by the wrong user.";
    return false;
  }

  if ((stat_info.st_mode & S_IWGRP) &&
      !ContainsKey(group_gids, stat_info.st_gid)) {
    DLOG(ERROR) << "Path " << path.value()
                << " is writable by an unprivileged group.";
    return false;
  }

  if (stat_info.st_mode & S_IWOTH) {
    DLOG(ERROR) << "Path " << path.value()
                << " is writable by any user.";
    return false;
  }

  return true;
}
