bool RenderThreadImpl::OnControlMessageReceived(const IPC::Message& msg) {
  ObserverListBase<RenderProcessObserver>::Iterator it(observers_);
  RenderProcessObserver* observer;
  while ((observer = it.GetNext()) != NULL) {
    if (observer->OnControlMessageReceived(msg))
      return true;
  }

  if (appcache_dispatcher_->OnMessageReceived(msg) ||
      dom_storage_dispatcher_->OnMessageReceived(msg) ||
      embedded_worker_dispatcher_->OnMessageReceived(msg)) {
    return true;
  }

  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(RenderThreadImpl, msg)
    IPC_MESSAGE_HANDLER(ViewMsg_SetZoomLevelForCurrentURL,
                        OnSetZoomLevelForCurrentURL)
    IPC_MESSAGE_HANDLER(ViewMsg_New, OnCreateNewView)
    IPC_MESSAGE_HANDLER(ViewMsg_PurgePluginListCache, OnPurgePluginListCache)
    IPC_MESSAGE_HANDLER(ViewMsg_NetworkTypeChanged, OnNetworkTypeChanged)
    IPC_MESSAGE_HANDLER(ViewMsg_TempCrashWithData, OnTempCrashWithData)
    IPC_MESSAGE_HANDLER(WorkerProcessMsg_CreateWorker, OnCreateNewSharedWorker)
    IPC_MESSAGE_HANDLER(ViewMsg_TimezoneChange, OnUpdateTimezone)
#if defined(OS_ANDROID)
    IPC_MESSAGE_HANDLER(ViewMsg_SetWebKitSharedTimersSuspended,
                        OnSetWebKitSharedTimersSuspended)
#endif
#if defined(OS_MACOSX)
    IPC_MESSAGE_HANDLER(ViewMsg_UpdateScrollbarTheme, OnUpdateScrollbarTheme)
#endif
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  return handled;
}
