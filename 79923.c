void FileEnumerator::GetFindInfo(FindInfo* info) {
  DCHECK(info);

  if (current_directory_entry_ >= directory_entries_.size())
    return;

  DirectoryEntryInfo* cur_entry = &directory_entries_[current_directory_entry_];
  memcpy(&(info->stat), &(cur_entry->stat), sizeof(info->stat));
  info->filename.assign(cur_entry->filename.value());
}
