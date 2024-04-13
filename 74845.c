void RenderView::OnToggleSpellPanel(bool is_currently_visible) {
  if (!webview())
    return;
  spelling_panel_visible_ = is_currently_visible;
  webview()->focusedFrame()->executeCommand(
      WebString::fromUTF8("ToggleSpellPanel"));
}
