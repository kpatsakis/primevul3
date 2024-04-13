void RenderView::OnCaptureSnapshot() {
  SkBitmap snapshot;
  bool error = false;

  WebFrame* main_frame = webview()->mainFrame();
  if (!main_frame)
    error = true;

  if (!error && !CaptureSnapshot(webview(), &snapshot))
    error = true;

  DCHECK(error == snapshot.empty()) <<
      "Snapshot should be empty on error, non-empty otherwise.";

  Send(new ViewHostMsg_Snapshot(routing_id_, snapshot));
}
