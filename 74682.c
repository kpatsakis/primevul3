AutoFillManager::AutoFillManager(TabContents* tab_contents)
    : tab_contents_(tab_contents),
      personal_data_(NULL),
      download_manager_(tab_contents_->profile()) {
  DCHECK(tab_contents);

  personal_data_ =
      tab_contents_->profile()->GetOriginalProfile()->GetPersonalDataManager();
  download_manager_.SetObserver(this);
}
