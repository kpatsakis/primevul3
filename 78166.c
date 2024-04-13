void WebContentsImpl::RequestToLockMouse(bool user_gesture,
                                         bool last_unlocked_by_target) {
  if (delegate_) {
    delegate_->RequestToLockMouse(this, user_gesture, last_unlocked_by_target);
  } else {
    GotResponseToLockMouseRequest(false);
  }
}
