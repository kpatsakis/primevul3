void SavePackage::OnReceivedSerializedHtmlData(const GURL& frame_url,
                                               const std::string& data,
                                               int32 status) {
  WebPageSerializerClient::PageSerializationStatus flag =
      static_cast<WebPageSerializerClient::PageSerializationStatus>(status);
  if (wait_state_ != HTML_DATA)
    return;

  int id = contents_id();
  if (flag == WebPageSerializerClient::AllFramesAreFinished) {
    for (SaveUrlItemMap::iterator it = in_progress_items_.begin();
         it != in_progress_items_.end(); ++it) {
      VLOG(20) << " " << __FUNCTION__ << "()"
               << " save_id = " << it->second->save_id()
               << " url = \"" << it->second->url().spec() << "\"";
      BrowserThread::PostTask(
          BrowserThread::FILE, FROM_HERE,
          base::Bind(&SaveFileManager::SaveFinished,
                     file_manager_,
                     it->second->save_id(),
                     it->second->url(),
                     id,
                     true));
    }
    return;
  }

  SaveUrlItemMap::iterator it = in_progress_items_.find(frame_url.spec());
  if (it == in_progress_items_.end()) {
    for (SavedItemMap::iterator saved_it = saved_success_items_.begin();
      saved_it != saved_success_items_.end(); ++saved_it) {
      if (saved_it->second->url() == frame_url) {
        wrote_to_completed_file_ = true;
        break;
      }
    }

    it = saved_failed_items_.find(frame_url.spec());
    if (it != saved_failed_items_.end())
      wrote_to_failed_file_ = true;

    return;
  }

  SaveItem* save_item = it->second;
  DCHECK(save_item->save_source() == SaveFileCreateInfo::SAVE_FILE_FROM_DOM);

  if (!data.empty()) {
    scoped_refptr<net::IOBuffer> new_data(new net::IOBuffer(data.size()));
    memcpy(new_data->data(), data.data(), data.size());

    BrowserThread::PostTask(
        BrowserThread::FILE, FROM_HERE,
        base::Bind(&SaveFileManager::UpdateSaveProgress,
                   file_manager_,
                   save_item->save_id(),
                   new_data,
                   static_cast<int>(data.size())));
  }

  if (flag == WebPageSerializerClient::CurrentFrameIsFinished) {
    VLOG(20) << " " << __FUNCTION__ << "()"
             << " save_id = " << save_item->save_id()
             << " url = \"" << save_item->url().spec() << "\"";
    BrowserThread::PostTask(
        BrowserThread::FILE, FROM_HERE,
        base::Bind(&SaveFileManager::SaveFinished,
                   file_manager_,
                   save_item->save_id(),
                   save_item->url(),
                   id,
                   true));
  }
}
