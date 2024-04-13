void SyncManager::SetPassphrase(const std::string& passphrase,
                                bool is_explicit,
                                bool user_provided) {
  DCHECK(thread_checker_.CalledOnValidThread());
  data_->SetPassphrase(passphrase, is_explicit, user_provided);
}
