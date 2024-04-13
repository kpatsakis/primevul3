static bool CreateTemporaryDirInDirImpl(const FilePath& base_dir,
                                        const FilePath::StringType& name_tmpl,
                                        FilePath* new_dir) {
  base::ThreadRestrictions::AssertIOAllowed();  // For call to mkdtemp().
  DCHECK(name_tmpl.find("XXXXXX") != FilePath::StringType::npos)
      << "Directory name template must contain \"XXXXXX\".";

  FilePath sub_dir = base_dir.Append(name_tmpl);
  std::string sub_dir_string = sub_dir.value();

  char* buffer = const_cast<char*>(sub_dir_string.c_str());
  char* dtemp = mkdtemp(buffer);
  if (!dtemp) {
    DPLOG(ERROR) << "mkdtemp";
    return false;
  }
  *new_dir = FilePath(dtemp);
  return true;
}
