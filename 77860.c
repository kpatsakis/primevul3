void RenderFrameImpl::OnSetEditableSelectionOffsets(int start, int end) {
  base::AutoReset<bool> handling_select_range(&handling_select_range_, true);
  if (!GetRenderWidget()->ShouldHandleImeEvent())
    return;
  ImeEventGuard guard(GetRenderWidget());
  frame_->setEditableSelectionOffsets(start, end);
}
