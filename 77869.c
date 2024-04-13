void RenderFrameImpl::PepperDidReceiveMouseEvent(
    PepperPluginInstanceImpl* instance) {
  render_view_->set_pepper_last_mouse_event_target(instance);
}
