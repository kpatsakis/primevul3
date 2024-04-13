void SavePackage::CheckFinish() {
  if (in_process_count() || finished_)
    return;

  FilePath dir = (save_type_ == SAVE_PAGE_TYPE_AS_COMPLETE_HTML &&
                  saved_success_items_.size() > 1) ?
                  saved_main_directory_path_ : FilePath();

  FinalNameList final_names;
  for (SavedItemMap::iterator it = saved_success_items_.begin();
      it != saved_success_items_.end(); ++it)
    final_names.push_back(std::make_pair(it->first,
                                         it->second->full_path()));

  BrowserThread::PostTask(
      BrowserThread::FILE, FROM_HERE,
      base::Bind(&SaveFileManager::RenameAllFiles,
                 file_manager_,
                 final_names,
                 dir,
                 web_contents()->GetRenderProcessHost()->GetID(),
                 web_contents()->GetRenderViewHost()->GetRoutingID(),
                 id()));
}
