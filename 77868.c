void RenderFrameImpl::PepperDidChangeCursor(
    PepperPluginInstanceImpl* instance,
    const blink::WebCursorInfo& cursor) {
  if (instance == render_view_->pepper_last_mouse_event_target())
    GetRenderWidget()->didChangeCursor(cursor);
}
