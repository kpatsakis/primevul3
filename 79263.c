void TabsCaptureVisibleTabFunction::SendInternalError() {
  error_ = keys::kInternalVisibleTabCaptureError;
  SendResponse(false);
}
