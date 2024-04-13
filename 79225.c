void TabsCaptureVisibleTabFunction::CopyFromBackingStoreComplete(
    bool succeeded,
    const SkBitmap& bitmap) {
  if (succeeded) {
    VLOG(1) << "captureVisibleTab() got image from backing store.";
    SendResultFromBitmap(bitmap);
    return;
  }

  WebContents* web_contents = NULL;
  if (!GetTabToCapture(&web_contents)) {
    SendInternalError();
    return;
  }

  content::RenderWidgetHost* render_widget_host =
      web_contents->GetRenderViewHost();
  if (!render_widget_host) {
    SendInternalError();
    return;
  }

  render_widget_host->GetSnapshotFromRenderer(
      gfx::Rect(),
      base::Bind(
          &TabsCaptureVisibleTabFunction::GetSnapshotFromRendererComplete,
          this));
}
