void RenderView::OnMessageReceived(const IPC::Message& message) {
  WebFrame* main_frame = webview() ? webview()->mainFrame() : NULL;
  if (main_frame)
    child_process_logging::SetActiveURL(main_frame->url());

  if (devtools_client_.get() && devtools_client_->OnMessageReceived(message))
    return;
  if (devtools_agent_.get() && devtools_agent_->OnMessageReceived(message))
    return;
  if (notification_provider_->OnMessageReceived(message))
    return;
  if (geolocation_dispatcher_.get() &&
      geolocation_dispatcher_->OnMessageReceived(message)) {
    return;
  }
  if (speech_input_dispatcher_.get() &&
      speech_input_dispatcher_->OnMessageReceived(message)) {
    return;
  }
  if (device_orientation_dispatcher_.get() &&
      device_orientation_dispatcher_->OnMessageReceived(message)) {
    return;
  }

  IPC_BEGIN_MESSAGE_MAP(RenderView, message)
    IPC_MESSAGE_HANDLER(ViewMsg_CaptureThumbnail, OnCaptureThumbnail)
    IPC_MESSAGE_HANDLER(ViewMsg_CaptureSnapshot, OnCaptureSnapshot)
    IPC_MESSAGE_HANDLER(ViewMsg_PrintPages, OnPrintPages)
    IPC_MESSAGE_HANDLER(ViewMsg_PrintingDone, OnPrintingDone)
    IPC_MESSAGE_HANDLER(ViewMsg_Navigate, OnNavigate)
    IPC_MESSAGE_HANDLER(ViewMsg_Stop, OnStop)
    IPC_MESSAGE_HANDLER(ViewMsg_ReloadFrame, OnReloadFrame)
    IPC_MESSAGE_HANDLER(ViewMsg_Undo, OnUndo)
    IPC_MESSAGE_HANDLER(ViewMsg_Redo, OnRedo)
    IPC_MESSAGE_HANDLER(ViewMsg_Cut, OnCut)
    IPC_MESSAGE_HANDLER(ViewMsg_Copy, OnCopy)
#if defined(OS_MACOSX)
    IPC_MESSAGE_HANDLER(ViewMsg_CopyToFindPboard, OnCopyToFindPboard)
#endif
    IPC_MESSAGE_HANDLER(ViewMsg_Paste, OnPaste)
    IPC_MESSAGE_HANDLER(ViewMsg_Replace, OnReplace)
    IPC_MESSAGE_HANDLER(ViewMsg_ToggleSpellPanel, OnToggleSpellPanel)
    IPC_MESSAGE_HANDLER(ViewMsg_AdvanceToNextMisspelling,
                        OnAdvanceToNextMisspelling)
    IPC_MESSAGE_HANDLER(ViewMsg_ToggleSpellCheck, OnToggleSpellCheck)
    IPC_MESSAGE_HANDLER(ViewMsg_Delete, OnDelete)
    IPC_MESSAGE_HANDLER(ViewMsg_SelectAll, OnSelectAll)
    IPC_MESSAGE_HANDLER(ViewMsg_CopyImageAt, OnCopyImageAt)
    IPC_MESSAGE_HANDLER(ViewMsg_ExecuteEditCommand, OnExecuteEditCommand)
    IPC_MESSAGE_HANDLER(ViewMsg_Find, OnFind)
    IPC_MESSAGE_HANDLER(ViewMsg_StopFinding, OnStopFinding)
    IPC_MESSAGE_HANDLER(ViewMsg_FindReplyACK, OnFindReplyAck)
    IPC_MESSAGE_HANDLER(ViewMsg_Zoom, OnZoom)
    IPC_MESSAGE_HANDLER(ViewMsg_SetContentSettingsForLoadingURL,
                        OnSetContentSettingsForLoadingURL)
    IPC_MESSAGE_HANDLER(ViewMsg_SetZoomLevelForLoadingURL,
                        OnSetZoomLevelForLoadingURL)
    IPC_MESSAGE_HANDLER(ViewMsg_SetPageEncoding, OnSetPageEncoding)
    IPC_MESSAGE_HANDLER(ViewMsg_ResetPageEncodingToDefault,
                        OnResetPageEncodingToDefault)
    IPC_MESSAGE_HANDLER(ViewMsg_SetupDevToolsClient, OnSetupDevToolsClient)
    IPC_MESSAGE_HANDLER(ViewMsg_DownloadFavIcon, OnDownloadFavIcon)
    IPC_MESSAGE_HANDLER(ViewMsg_ScriptEvalRequest, OnScriptEvalRequest)
    IPC_MESSAGE_HANDLER(ViewMsg_CSSInsertRequest, OnCSSInsertRequest)
    IPC_MESSAGE_HANDLER(ViewMsg_AddMessageToConsole, OnAddMessageToConsole)
    IPC_MESSAGE_HANDLER(ViewMsg_ReservePageIDRange, OnReservePageIDRange)
    IPC_MESSAGE_HANDLER(ViewMsg_FillPasswordForm, OnFillPasswordForm)
    IPC_MESSAGE_HANDLER(ViewMsg_DragTargetDragEnter, OnDragTargetDragEnter)
    IPC_MESSAGE_HANDLER(ViewMsg_DragTargetDragOver, OnDragTargetDragOver)
    IPC_MESSAGE_HANDLER(ViewMsg_DragTargetDragLeave, OnDragTargetDragLeave)
    IPC_MESSAGE_HANDLER(ViewMsg_DragTargetDrop, OnDragTargetDrop)
    IPC_MESSAGE_HANDLER(ViewMsg_AllowBindings, OnAllowBindings)
    IPC_MESSAGE_HANDLER(ViewMsg_SetDOMUIProperty, OnSetDOMUIProperty)
    IPC_MESSAGE_HANDLER(ViewMsg_DragSourceEndedOrMoved,
                        OnDragSourceEndedOrMoved)
    IPC_MESSAGE_HANDLER(ViewMsg_DragSourceSystemDragEnded,
                        OnDragSourceSystemDragEnded)
    IPC_MESSAGE_HANDLER(ViewMsg_SetInitialFocus, OnSetInitialFocus)
    IPC_MESSAGE_HANDLER(ViewMsg_UpdateTargetURL_ACK, OnUpdateTargetURLAck)
    IPC_MESSAGE_HANDLER(ViewMsg_UpdateWebPreferences, OnUpdateWebPreferences)
    IPC_MESSAGE_HANDLER(ViewMsg_SetAltErrorPageURL, OnSetAltErrorPageURL)
    IPC_MESSAGE_HANDLER(ViewMsg_InstallMissingPlugin, OnInstallMissingPlugin)
    IPC_MESSAGE_HANDLER(ViewMsg_LoadBlockedPlugins, OnLoadBlockedPlugins)
    IPC_MESSAGE_HANDLER(ViewMsg_RunFileChooserResponse, OnFileChooserResponse)
    IPC_MESSAGE_HANDLER(ViewMsg_EnableViewSourceMode, OnEnableViewSourceMode)
    IPC_MESSAGE_HANDLER(ViewMsg_GetAllSavableResourceLinksForCurrentPage,
                        OnGetAllSavableResourceLinksForCurrentPage)
    IPC_MESSAGE_HANDLER(
        ViewMsg_GetSerializedHtmlDataForCurrentPageWithLocalLinks,
        OnGetSerializedHtmlDataForCurrentPageWithLocalLinks)
    IPC_MESSAGE_HANDLER(ViewMsg_GetApplicationInfo, OnGetApplicationInfo)
    IPC_MESSAGE_HANDLER(ViewMsg_ShouldClose, OnShouldClose)
    IPC_MESSAGE_HANDLER(ViewMsg_ClosePage, OnClosePage)
    IPC_MESSAGE_HANDLER(ViewMsg_ThemeChanged, OnThemeChanged)
    IPC_MESSAGE_HANDLER(ViewMsg_HandleMessageFromExternalHost,
                        OnHandleMessageFromExternalHost)
    IPC_MESSAGE_HANDLER(ViewMsg_DisassociateFromPopupCount,
                        OnDisassociateFromPopupCount)
    IPC_MESSAGE_HANDLER(ViewMsg_AutoFillSuggestionsReturned,
                        OnAutoFillSuggestionsReturned)
    IPC_MESSAGE_HANDLER(ViewMsg_AutoFillFormDataFilled,
                        OnAutoFillFormDataFilled)
    IPC_MESSAGE_HANDLER(ViewMsg_AllowScriptToClose,
                        OnAllowScriptToClose)
    IPC_MESSAGE_HANDLER(ViewMsg_MoveOrResizeStarted, OnMoveOrResizeStarted)
    IPC_MESSAGE_HANDLER(ViewMsg_ExtensionResponse, OnExtensionResponse)
    IPC_MESSAGE_HANDLER(ViewMsg_ExtensionMessageInvoke,
                        OnExtensionMessageInvoke)
    IPC_MESSAGE_HANDLER(ViewMsg_ClearFocusedNode, OnClearFocusedNode)
    IPC_MESSAGE_HANDLER(ViewMsg_SetBackground, OnSetBackground)
    IPC_MESSAGE_HANDLER(ViewMsg_EnablePreferredSizeChangedMode,
                        OnEnablePreferredSizeChangedMode)
    IPC_MESSAGE_HANDLER(ViewMsg_DisableScrollbarsForSmallWindows,
                        OnDisableScrollbarsForSmallWindows)
    IPC_MESSAGE_HANDLER(ViewMsg_SetRendererPrefs, OnSetRendererPrefs)
    IPC_MESSAGE_HANDLER(ViewMsg_UpdateBrowserWindowId,
                        OnUpdateBrowserWindowId)
    IPC_MESSAGE_HANDLER(ViewMsg_NotifyRenderViewType,
                        OnNotifyRendererViewType)
    IPC_MESSAGE_HANDLER(ViewMsg_MediaPlayerActionAt, OnMediaPlayerActionAt)
    IPC_MESSAGE_HANDLER(ViewMsg_SetActive, OnSetActive)
#if defined(OS_MACOSX)
    IPC_MESSAGE_HANDLER(ViewMsg_SetWindowVisibility, OnSetWindowVisibility)
    IPC_MESSAGE_HANDLER(ViewMsg_WindowFrameChanged, OnWindowFrameChanged)
#endif
    IPC_MESSAGE_HANDLER(ViewMsg_SetEditCommandsForNextKeyEvent,
                        OnSetEditCommandsForNextKeyEvent)
    IPC_MESSAGE_HANDLER(ViewMsg_ExecuteCode,
                        OnExecuteCode)
    IPC_MESSAGE_HANDLER(ViewMsg_CustomContextMenuAction,
                        OnCustomContextMenuAction)
    IPC_MESSAGE_HANDLER(ViewMsg_TranslatePage, OnTranslatePage)
    IPC_MESSAGE_HANDLER(ViewMsg_RevertTranslation, OnRevertTranslation)
    IPC_MESSAGE_HANDLER(ViewMsg_GetAccessibilityTree, OnGetAccessibilityTree)
    IPC_MESSAGE_HANDLER(ViewMsg_SetAccessibilityFocus, OnSetAccessibilityFocus)
    IPC_MESSAGE_HANDLER(ViewMsg_AccessibilityDoDefaultAction,
                        OnAccessibilityDoDefaultAction)

    IPC_MESSAGE_UNHANDLED(RenderWidget::OnMessageReceived(message))
  IPC_END_MESSAGE_MAP()
}
