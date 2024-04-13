WebContentsImpl::~WebContentsImpl() {
  is_being_destroyed_ = true;

  ClearAllPowerSaveBlockers();

  for (std::set<RenderWidgetHostImpl*>::iterator iter =
           created_widgets_.begin(); iter != created_widgets_.end(); ++iter) {
    (*iter)->DetachDelegate();
  }
  created_widgets_.clear();

  if (dialog_manager_)
    dialog_manager_->WebContentsDestroyed(this);

  if (color_chooser_)
    color_chooser_->End();

  NotifyDisconnected();

  NotificationService::current()->Notify(
      NOTIFICATION_WEB_CONTENTS_DESTROYED,
      Source<WebContents>(this),
      NotificationService::NoDetails());

#if defined(OS_WIN) && !defined(USE_AURA)
  if (view_->GetNativeView()) {
    RenderViewHost* host = GetRenderViewHost();
    if (host && host->GetView())
      RenderWidgetHostViewPort::FromRWHV(host->GetView())->WillWmDestroy();
  }
#endif

  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    WebContentsImplDestroyed());

  SetDelegate(NULL);

  STLDeleteContainerPairSecondPointers(destruction_observers_.begin(),
                                       destruction_observers_.end());
}
