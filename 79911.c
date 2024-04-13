bool CreateDirectory(const FilePath& full_path) {
  base::ThreadRestrictions::AssertIOAllowed();  // For call to mkdir().
  std::vector<FilePath> subpaths;

  FilePath last_path = full_path;
  subpaths.push_back(full_path);
  for (FilePath path = full_path.DirName();
       path.value() != last_path.value(); path = path.DirName()) {
    subpaths.push_back(path);
    last_path = path;
  }

  for (std::vector<FilePath>::reverse_iterator i = subpaths.rbegin();
       i != subpaths.rend(); ++i) {
    if (DirectoryExists(*i))
      continue;
    if (mkdir(i->value().c_str(), 0700) == 0)
      continue;
    if (!DirectoryExists(*i))
      return false;
  }
  return true;
}
