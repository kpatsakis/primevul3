void TabsCaptureVisibleTabFunction::GetSnapshotFromRendererComplete(
    bool succeeded,
    const SkBitmap& bitmap) {
  if (!succeeded) {
    SendInternalError();
  } else {
    VLOG(1) << "captureVisibleTab() got image from renderer.";
    SendResultFromBitmap(bitmap);
  }
}
