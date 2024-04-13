void RenderView::didChangeSelection(bool is_empty_selection) {
#if defined(USE_X11)
  if (!handling_input_event_)
      return;
  if (!is_empty_selection) {
    const std::string& this_selection =
        webview()->focusedFrame()->selectionAsText().utf8();
    if (this_selection == last_selection_)
      return;

    Send(new ViewHostMsg_SelectionChanged(routing_id_,
         this_selection));
    last_selection_ = this_selection;
  } else {
    last_selection_.clear();
  }
#endif
}
