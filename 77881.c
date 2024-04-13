void RenderFrameImpl::SyncSelectionIfRequired() {
  base::string16 text;
  size_t offset;
  gfx::Range range;
#if defined(ENABLE_PLUGINS)
  if (render_view_->focused_pepper_plugin_) {
    render_view_->focused_pepper_plugin_->GetSurroundingText(&text, &range);
    offset = 0;  // Pepper API does not support offset reporting.
  } else
#endif
  {
    size_t location, length;
    if (!GetRenderWidget()->webwidget()->caretOrSelectionRange(
            &location, &length)) {
      return;
    }

    range = gfx::Range(location, location + length);

    if (GetRenderWidget()->webwidget()->textInputInfo().type !=
            blink::WebTextInputTypeNone) {
      if (location > kExtraCharsBeforeAndAfterSelection)
        offset = location - kExtraCharsBeforeAndAfterSelection;
      else
        offset = 0;
      length = location + length - offset + kExtraCharsBeforeAndAfterSelection;
      WebRange webrange = WebRange::fromDocumentRange(frame_, offset, length);
      if (!webrange.isNull())
        text = WebRange::fromDocumentRange(
            frame_, offset, length).toPlainText();
    } else {
      offset = location;
      text = frame_->selectionAsText();
      range.set_end(range.start() + text.length());
    }
  }

  if (selection_text_offset_ != offset ||
      selection_range_ != range ||
      selection_text_ != text) {
    selection_text_ = text;
    selection_text_offset_ = offset;
    selection_range_ = range;
    Send(new ViewHostMsg_SelectionChanged(
        GetRenderWidget()->routing_id(), text, offset, range));
  }
  GetRenderWidget()->UpdateSelectionBounds();
}
