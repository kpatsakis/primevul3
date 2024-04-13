void RenderFrameImpl::didChangeSelection(bool is_empty_selection) {
  if (!GetRenderWidget()->handling_input_event() && !handling_select_range_)
    return;

  if (is_empty_selection)
    selection_text_.clear();

  GetRenderWidget()->UpdateTextInputType();
  SyncSelectionIfRequired();
#if defined(OS_ANDROID)
  GetRenderWidget()->UpdateTextInputState(RenderWidget::NO_SHOW_IME,
                                          RenderWidget::FROM_NON_IME);
#endif
}
