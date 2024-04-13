bool WebContentsImpl::OnMessageReceived(RenderViewHost* render_view_host,
                                        const IPC::Message& message) {
  if (GetWebUI() &&
      static_cast<WebUIImpl*>(GetWebUI())->OnMessageReceived(message)) {
    return true;
  }

  ObserverListBase<WebContentsObserver>::Iterator it(observers_);
  WebContentsObserver* observer;
  while ((observer = it.GetNext()) != NULL)
    if (observer->OnMessageReceived(message))
      return true;

  message_source_ = render_view_host;
  bool handled = true;
  bool message_is_ok = true;
  IPC_BEGIN_MESSAGE_MAP_EX(WebContentsImpl, message, message_is_ok)
    IPC_MESSAGE_HANDLER(ViewHostMsg_DidLoadResourceFromMemoryCache,
                        OnDidLoadResourceFromMemoryCache)
    IPC_MESSAGE_HANDLER(ViewHostMsg_DidDisplayInsecureContent,
                        OnDidDisplayInsecureContent)
    IPC_MESSAGE_HANDLER(ViewHostMsg_DidRunInsecureContent,
                        OnDidRunInsecureContent)
    IPC_MESSAGE_HANDLER(ViewHostMsg_DocumentLoadedInFrame,
                        OnDocumentLoadedInFrame)
    IPC_MESSAGE_HANDLER(ViewHostMsg_DidFinishLoad, OnDidFinishLoad)
    IPC_MESSAGE_HANDLER(ViewHostMsg_DidFailLoadWithError,
                        OnDidFailLoadWithError)
    IPC_MESSAGE_HANDLER(ViewHostMsg_GoToEntryAtOffset, OnGoToEntryAtOffset)
    IPC_MESSAGE_HANDLER(ViewHostMsg_UpdateZoomLimits, OnUpdateZoomLimits)
    IPC_MESSAGE_HANDLER(ViewHostMsg_EnumerateDirectory, OnEnumerateDirectory)
    IPC_MESSAGE_HANDLER(ViewHostMsg_JSOutOfMemory, OnJSOutOfMemory)
    IPC_MESSAGE_HANDLER(ViewHostMsg_RegisterProtocolHandler,
                        OnRegisterProtocolHandler)
    IPC_MESSAGE_HANDLER(ViewHostMsg_Find_Reply, OnFindReply)
    IPC_MESSAGE_HANDLER(ViewHostMsg_DidProgrammaticallyScroll,
                        OnDidProgrammaticallyScroll)
    IPC_MESSAGE_HANDLER(ViewHostMsg_CrashedPlugin, OnCrashedPlugin)
    IPC_MESSAGE_HANDLER(ViewHostMsg_AppCacheAccessed, OnAppCacheAccessed)
    IPC_MESSAGE_HANDLER(ViewHostMsg_OpenColorChooser, OnOpenColorChooser)
    IPC_MESSAGE_HANDLER(ViewHostMsg_EndColorChooser, OnEndColorChooser)
    IPC_MESSAGE_HANDLER(ViewHostMsg_SetSelectedColorInColorChooser,
                        OnSetSelectedColorInColorChooser)
    IPC_MESSAGE_HANDLER(ViewHostMsg_PepperPluginHung, OnPepperPluginHung)
    IPC_MESSAGE_HANDLER(ViewHostMsg_WebUISend, OnWebUISend)
    IPC_MESSAGE_HANDLER(ViewHostMsg_RequestPpapiBrokerPermission,
                        OnRequestPpapiBrokerPermission)
    IPC_MESSAGE_HANDLER_GENERIC(BrowserPluginHostMsg_AllocateInstanceID,
                                OnBrowserPluginMessage(message))
    IPC_MESSAGE_HANDLER_GENERIC(BrowserPluginHostMsg_Attach,
                                OnBrowserPluginMessage(message))
    IPC_MESSAGE_HANDLER(ImageHostMsg_DidDownloadImage, OnDidDownloadImage)
    IPC_MESSAGE_HANDLER(ViewHostMsg_UpdateFaviconURL, OnUpdateFaviconURL)
#if defined(OS_ANDROID)
    IPC_MESSAGE_HANDLER(ViewHostMsg_FindMatchRects_Reply,
                        OnFindMatchRectsReply)
    IPC_MESSAGE_HANDLER(ViewHostMsg_OpenDateTimeDialog,
                        OnOpenDateTimeDialog)
#endif
    IPC_MESSAGE_HANDLER(ViewHostMsg_FrameAttached, OnFrameAttached)
    IPC_MESSAGE_HANDLER(ViewHostMsg_FrameDetached, OnFrameDetached)
    IPC_MESSAGE_HANDLER(ViewHostMsg_MediaNotification, OnMediaNotification)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP_EX()
  message_source_ = NULL;

  if (!message_is_ok) {
    RecordAction(UserMetricsAction("BadMessageTerminate_RVD"));
    GetRenderProcessHost()->ReceivedBadMessage();
  }

  return handled;
}
