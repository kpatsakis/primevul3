void RenderView::OnFind(int request_id, const string16& search_text,
                        const WebFindOptions& options) {
  WebFrame* main_frame = webview()->mainFrame();

  if (main_frame->document().isPluginDocument()) {
    if (options.findNext) {
      GetWebPluginFromPluginDocument()->selectFindResult(options.forward);
    } else {
      if (GetWebPluginFromPluginDocument()->startFind(
          search_text, options.matchCase, request_id)) {
      } else {
        ReportNoFindInPageResults(request_id);
      }
    }
    return;
  }

  WebFrame* frame_after_main = main_frame->traverseNext(true);
  WebFrame* focused_frame = webview()->focusedFrame();
  WebFrame* search_frame = focused_frame;  // start searching focused frame.

  bool multi_frame = (frame_after_main != main_frame);

  bool wrap_within_frame = !multi_frame;

  WebRect selection_rect;
  bool result = false;

  WebRange current_selection = focused_frame->selectionRange();

  do {
    result = search_frame->find(
        request_id, search_text, options, wrap_within_frame, &selection_rect);

    if (!result) {
      search_frame->executeCommand(WebString::fromUTF8("Unselect"));

      do {
        search_frame = options.forward ?
            search_frame->traverseNext(true) :
            search_frame->traversePrevious(true);
      } while (!search_frame->hasVisibleContent() &&
               search_frame != focused_frame);

      search_frame->executeCommand(WebString::fromUTF8("Unselect"));

      if (multi_frame && search_frame == focused_frame) {
        result = search_frame->find(
            request_id, search_text, options, true,  // Force wrapping.
            &selection_rect);
      }
    }

    webview()->setFocusedFrame(search_frame);
  } while (!result && search_frame != focused_frame);

  if (options.findNext && current_selection.isNull()) {
    main_frame->increaseMatchCount(0, request_id);
  } else {
    int ordinal = result ? -1 : 0;  // -1 here means, we might know more later.
    int match_count = result ? 1 : 0;  // 1 here means possibly more coming.

    bool final_status_update = !result;

    Send(new ViewHostMsg_Find_Reply(routing_id_,
                                    request_id,
                                    match_count,
                                    selection_rect,
                                    ordinal,
                                    final_status_update));

    search_frame = main_frame;

    main_frame->resetMatchCount();

    do {
      search_frame->cancelPendingScopingEffort();

      if (result) {
        search_frame->scopeStringMatches(request_id,
                                         search_text,
                                         options,
                                         true);  // reset the tickmarks
      }

      search_frame = search_frame->traverseNext(true);
    } while (search_frame != main_frame);
  }
}
