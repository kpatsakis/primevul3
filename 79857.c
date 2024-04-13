bool SavePackage::Init(
    const SavePackageDownloadCreatedCallback& download_created_callback) {
  if (wait_state_ != INITIALIZE)
    return false;

  wait_state_ = START_PROCESS;

  BrowserContext* browser_context = web_contents()->GetBrowserContext();
  if (!browser_context) {
    NOTREACHED();
    return false;
  }

  download_ = download_manager_->CreateSavePackageDownloadItem(
      saved_main_file_path_,
      page_url_,
      ((save_type_ == SAVE_PAGE_TYPE_AS_MHTML) ?
       "multipart/related" : "text/html"),
      this);
  if (!download_created_callback.is_null())
    download_created_callback.Run(download_);

  if (save_type_ == SAVE_PAGE_TYPE_AS_COMPLETE_HTML) {
    DCHECK(!saved_main_directory_path_.empty());
    GetAllSavableResourceLinksForCurrentPage();
  } else if (save_type_ == SAVE_PAGE_TYPE_AS_MHTML) {
    web_contents()->GenerateMHTML(saved_main_file_path_, base::Bind(
        &SavePackage::OnMHTMLGenerated, this));
  } else {
    wait_state_ = NET_FILES;
    SaveFileCreateInfo::SaveFileSource save_source = page_url_.SchemeIsFile() ?
        SaveFileCreateInfo::SAVE_FILE_FROM_FILE :
        SaveFileCreateInfo::SAVE_FILE_FROM_NET;
    SaveItem* save_item = new SaveItem(page_url_,
                                       Referrer(),
                                       this,
                                       save_source);
    waiting_item_queue_.push(save_item);
    all_save_items_count_ = 1;
    download_->SetTotalBytes(1);

    DoSavingProcess();
  }

  return true;
}
