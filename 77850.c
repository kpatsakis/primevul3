void RenderFrameImpl::OnExtendSelectionAndDelete(int before, int after) {
  if (!GetRenderWidget()->ShouldHandleImeEvent())
    return;
  ImeEventGuard guard(GetRenderWidget());
  frame_->extendSelectionAndDelete(before, after);
}
