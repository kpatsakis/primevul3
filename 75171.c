bool ChildProcessSecurityPolicy::CanUploadFile(int renderer_id,
                                               const FilePath& file) {
  AutoLock lock(lock_);

  SecurityStateMap::iterator state = security_state_.find(renderer_id);
  if (state == security_state_.end())
    return false;

  return state->second->CanUploadFile(file);
}
