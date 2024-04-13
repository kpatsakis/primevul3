WebView* RenderView::createView(
    WebFrame* creator,
    const WebWindowFeatures& features,
    const WebString& frame_name) {
  if (shared_popup_counter_->data > kMaximumNumberOfUnacknowledgedPopups)
    return NULL;

  script_can_close_ = false;

  ViewHostMsg_CreateWindow_Params params;
  params.opener_id = routing_id_;
  params.user_gesture = creator->isProcessingUserGesture();
  params.window_container_type = WindowFeaturesToContainerType(features);
  params.session_storage_namespace_id = session_storage_namespace_id_;
  params.frame_name = frame_name;

  int32 routing_id = MSG_ROUTING_NONE;
  int64 cloned_session_storage_namespace_id;
  bool opener_suppressed = creator->willSuppressOpenerInNewFrame();

  render_thread_->Send(
      new ViewHostMsg_CreateWindow(params,
                                   &routing_id,
                                   &cloned_session_storage_namespace_id));
  if (routing_id == MSG_ROUTING_NONE)
    return NULL;

  RenderView* view = RenderView::Create(render_thread_,
                                        0,
                                        routing_id_,
                                        renderer_preferences_,
                                        webkit_preferences_,
                                        shared_popup_counter_,
                                        routing_id,
                                        cloned_session_storage_namespace_id,
                                        frame_name);
  view->opened_by_user_gesture_ = params.user_gesture;

  view->opener_suppressed_ = opener_suppressed;

  GURL creator_url(creator->securityOrigin().toString().utf8());
  if (!creator_url.is_valid() || !creator_url.IsStandard())
    creator_url = GURL();
  view->creator_url_ = creator_url;

  view->alternate_error_page_url_ = alternate_error_page_url_;

  return view->webview();
}
