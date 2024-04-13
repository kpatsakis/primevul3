void WebContentsImpl::SetIsLoading(bool is_loading,
                                   LoadNotificationDetails* details) {
  if (is_loading == is_loading_)
    return;

  if (!is_loading) {
    load_state_ = net::LoadStateWithParam(net::LOAD_STATE_IDLE, string16());
    load_state_host_.clear();
    upload_size_ = 0;
    upload_position_ = 0;
  }

  render_manager_.SetIsLoading(is_loading);

  is_loading_ = is_loading;
  waiting_for_response_ = is_loading;

  if (delegate_)
    delegate_->LoadingStateChanged(this);
  NotifyNavigationStateChanged(INVALIDATE_TYPE_LOAD);

  if (is_loading)
    TRACE_EVENT_ASYNC_BEGIN0("browser", "WebContentsImpl Loading", this);
  else
    TRACE_EVENT_ASYNC_END0("browser", "WebContentsImpl Loading", this);
  int type = is_loading ? NOTIFICATION_LOAD_START : NOTIFICATION_LOAD_STOP;
  NotificationDetails det = NotificationService::NoDetails();
  if (details)
      det = Details<LoadNotificationDetails>(details);
  NotificationService::current()->Notify(
      type, Source<NavigationController>(&controller_), det);
}
