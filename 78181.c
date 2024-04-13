void WebContentsImpl::SetParentNativeViewAccessible(
gfx::NativeViewAccessible accessible_parent) {
  accessible_parent_ = accessible_parent;
  if (GetRenderViewHost())
    GetRenderViewHostImpl()->SetParentNativeViewAccessible(accessible_parent);
}
