bool WebContentsImpl::PreHandleWheelEvent(
    const WebKit::WebMouseWheelEvent& event) {
#if !defined(OS_MACOSX)
  if (delegate_ &&
      event.wheelTicksY &&
      (event.modifiers & WebKit::WebInputEvent::ControlKey)) {
    delegate_->ContentsZoomChange(event.wheelTicksY > 0);
    return true;
  }
#endif

  return false;
}
