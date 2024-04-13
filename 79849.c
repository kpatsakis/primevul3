bool SavePackage::GenerateFileName(const std::string& disposition,
                                   const GURL& url,
                                   bool need_html_ext,
                                   FilePath::StringType* generated_name) {
  FilePath file_path = net::GenerateFileName(url, disposition, "", "", "",
                                             kDefaultSaveName);

  DCHECK(!file_path.empty());
  FilePath::StringType pure_file_name =
      file_path.RemoveExtension().BaseName().value();
  FilePath::StringType file_name_ext = file_path.Extension();

  if (need_html_ext) {
    file_name_ext = FILE_PATH_LITERAL(".");
    file_name_ext.append(kDefaultHtmlExtension);
  }

  uint32 max_path = GetMaxPathLengthForDirectory(saved_main_directory_path_);

  if (!GetSafePureFileName(saved_main_directory_path_, file_name_ext,
                           max_path, &pure_file_name))
    return false;

  FilePath::StringType file_name = pure_file_name + file_name_ext;

  FileNameSet::const_iterator iter = file_name_set_.find(file_name);
  if (iter == file_name_set_.end()) {
    file_name_set_.insert(file_name);
  } else {
    pure_file_name =
        FilePath(*iter).RemoveExtension().BaseName().value();
    FilePath::StringType base_file_name = StripOrdinalNumber(pure_file_name);

    if (!GetSafePureFileName(saved_main_directory_path_, file_name_ext,
        max_path - kMaxFileOrdinalNumberPartLength, &base_file_name))
      return false;

    uint32 ordinal_number;
    FileNameCountMap::iterator it = file_name_count_map_.find(base_file_name);
    if (it == file_name_count_map_.end()) {
      file_name_count_map_[base_file_name] = 1;
      ordinal_number = 1;
    } else {
      ordinal_number = it->second;
    }

    if (ordinal_number > (kMaxFileOrdinalNumber - 1)) {
      FilePath temp_file;
      file_util::CreateTemporaryFile(&temp_file);
      file_name = temp_file.RemoveExtension().BaseName().value();
      if (!GetSafePureFileName(saved_main_directory_path_,
                               FilePath::StringType(),
                               max_path, &file_name))
        return false;
    } else {
      for (int i = ordinal_number; i < kMaxFileOrdinalNumber; ++i) {
        FilePath::StringType new_name = base_file_name +
            StringPrintf(FILE_PATH_LITERAL("(%d)"), i) + file_name_ext;
        if (file_name_set_.find(new_name) == file_name_set_.end()) {
          file_name = new_name;
          file_name_count_map_[base_file_name] = ++i;
          break;
        }
      }
    }

    file_name_set_.insert(file_name);
  }

  DCHECK(!file_name.empty());
  generated_name->assign(file_name);

  return true;
}
