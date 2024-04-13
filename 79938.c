bool FileEnumerator::ReadDirectory(std::vector<DirectoryEntryInfo>* entries,
                                   const FilePath& source, bool show_links) {
  base::ThreadRestrictions::AssertIOAllowed();
  DIR* dir = opendir(source.value().c_str());
  if (!dir)
    return false;

#if !defined(OS_LINUX) && !defined(OS_MACOSX) && !defined(OS_BSD) && \
    !defined(OS_SOLARIS) && !defined(OS_ANDROID)
  #error Port warning: depending on the definition of struct dirent, \
         additional space for pathname may be needed
#endif

  struct dirent dent_buf;
  struct dirent* dent;
  while (readdir_r(dir, &dent_buf, &dent) == 0 && dent) {
    DirectoryEntryInfo info;
    info.filename = FilePath(dent->d_name);

    FilePath full_name = source.Append(dent->d_name);
    int ret;
    if (show_links)
      ret = lstat(full_name.value().c_str(), &info.stat);
    else
      ret = stat(full_name.value().c_str(), &info.stat);
    if (ret < 0) {
      if (!(errno == ENOENT && !show_links)) {
        DPLOG(ERROR) << "Couldn't stat "
                     << source.Append(dent->d_name).value();
      }
      memset(&info.stat, 0, sizeof(info.stat));
    }
    entries->push_back(info);
  }

  closedir(dir);
  return true;
}
