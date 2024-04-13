bool RenderViewImpl::HasTouchEventHandlersAt(const gfx::Point& point) const {
  if (!webview())
    return false;
  return webview()->hasTouchEventHandlersAt(point);
}
