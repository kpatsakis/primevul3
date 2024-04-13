void SavePackage::GetSerializedHtmlDataForCurrentPageWithLocalLinks() {
  if (wait_state_ != HTML_DATA)
    return;
  std::vector<GURL> saved_links;
  std::vector<FilePath> saved_file_paths;
  int successful_started_items_count = 0;

  for (SaveUrlItemMap::iterator it = in_progress_items_.begin();
       it != in_progress_items_.end(); ++it) {
    DCHECK(it->second->save_source() ==
           SaveFileCreateInfo::SAVE_FILE_FROM_DOM);
    if (it->second->has_final_name())
      successful_started_items_count++;
    saved_links.push_back(it->second->url());
    saved_file_paths.push_back(it->second->file_name());
  }

  if (successful_started_items_count != in_process_count())
    return;

  for (SavedItemMap::iterator it = saved_success_items_.begin();
       it != saved_success_items_.end(); ++it) {
    DCHECK(it->second->has_final_name());
    saved_links.push_back(it->second->url());
    saved_file_paths.push_back(it->second->file_name());
  }

  FilePath relative_dir_name = saved_main_directory_path_.BaseName();

  Send(new ViewMsg_GetSerializedHtmlDataForCurrentPageWithLocalLinks(
      routing_id(), saved_links, saved_file_paths, relative_dir_name));
}
