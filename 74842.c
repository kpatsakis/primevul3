void RenderView::OnStopFinding(const ViewMsg_StopFinding_Params& params) {
  WebView* view = webview();
  if (!view)
    return;

  WebDocument doc = view->mainFrame()->document();
  if (doc.isPluginDocument()) {
    GetWebPluginFromPluginDocument()->stopFind();
    return;
  }

  bool clear_selection =
      params.action == ViewMsg_StopFinding_Params::kClearSelection;
  if (clear_selection)
    view->focusedFrame()->executeCommand(WebString::fromUTF8("Unselect"));

  WebFrame* frame = view->mainFrame();
  while (frame) {
    frame->stopFinding(clear_selection);
    frame = frame->traverseNext(false);
  }

  if (params.action == ViewMsg_StopFinding_Params::kActivateSelection) {
    WebFrame* focused_frame = view->focusedFrame();
    if (focused_frame) {
      WebDocument doc = focused_frame->document();
      if (!doc.isNull()) {
        WebNode node = doc.focusedNode();
        if (!node.isNull())
          node.simulateClick();
      }
    }
  }
}
