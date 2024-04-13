FilePath FileEnumerator::Next() {
  ++current_directory_entry_;

  while (current_directory_entry_ >= directory_entries_.size()) {
    if (pending_paths_.empty())
      return FilePath();

    root_path_ = pending_paths_.top();
    root_path_ = root_path_.StripTrailingSeparators();
    pending_paths_.pop();

    std::vector<DirectoryEntryInfo> entries;
    if (!ReadDirectory(&entries, root_path_, file_type_ & SHOW_SYM_LINKS))
      continue;

    directory_entries_.clear();
    current_directory_entry_ = 0;
    for (std::vector<DirectoryEntryInfo>::const_iterator
        i = entries.begin(); i != entries.end(); ++i) {
      FilePath full_path = root_path_.Append(i->filename);
      if (ShouldSkip(full_path))
        continue;

      if (pattern_.size() &&
          fnmatch(pattern_.c_str(), full_path.value().c_str(), FNM_NOESCAPE))
        continue;

      if (recursive_ && S_ISDIR(i->stat.st_mode))
        pending_paths_.push(full_path);

      if ((S_ISDIR(i->stat.st_mode) && (file_type_ & DIRECTORIES)) ||
          (!S_ISDIR(i->stat.st_mode) && (file_type_ & FILES)))
        directory_entries_.push_back(*i);
    }
  }

  return root_path_.Append(directory_entries_[current_directory_entry_
      ].filename);
}
