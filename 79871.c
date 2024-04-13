void SavePackage::StartSave(const SaveFileCreateInfo* info) {
  DCHECK(info && !info->url.is_empty());

  SaveUrlItemMap::iterator it = in_progress_items_.find(info->url.spec());
  if (it == in_progress_items_.end()) {
    DCHECK(canceled());
    return;
  }
  SaveItem* save_item = it->second;

  DCHECK(!saved_main_file_path_.empty());

  save_item->SetSaveId(info->save_id);
  save_item->SetTotalBytes(info->total_bytes);

  DCHECK(!save_item->has_final_name());
  if (info->url != page_url_) {
    FilePath::StringType generated_name;
    bool need_html_ext =
        info->save_source == SaveFileCreateInfo::SAVE_FILE_FROM_DOM;
    if (!GenerateFileName(info->content_disposition,
                          GURL(info->url),
                          need_html_ext,
                          &generated_name)) {
      if (info->save_source == SaveFileCreateInfo::SAVE_FILE_FROM_DOM)
        Cancel(true);
      else
        SaveFinished(save_item->save_id(), 0, false);
      return;
    }

    DCHECK(save_type_ == SAVE_PAGE_TYPE_AS_COMPLETE_HTML);
    DCHECK(!saved_main_directory_path_.empty());

    FilePath final_name = saved_main_directory_path_.Append(generated_name);
    save_item->Rename(final_name);
  } else {
    save_item->Rename(saved_main_file_path_);
  }

  if (info->save_source == SaveFileCreateInfo::SAVE_FILE_FROM_FILE) {
    BrowserThread::PostTask(
        BrowserThread::FILE, FROM_HERE,
        base::Bind(&SaveFileManager::SaveLocalFile,
                   file_manager_,
                   save_item->url(),
                   save_item->save_id(),
                   contents_id()));
    return;
  }

  if (save_type_ == SAVE_PAGE_TYPE_AS_COMPLETE_HTML &&
      wait_state_ == HTML_DATA) {
    GetSerializedHtmlDataForCurrentPageWithLocalLinks();
  }
}
