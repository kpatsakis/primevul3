bool PrintWebViewHelper::PrintPages(const PrintMsg_PrintPages_Params& params,
                                    WebFrame* frame,
                                    const WebNode& node,
                                    PrepareFrameAndViewForPrint* prepare) {
  PrintMsg_Print_Params print_params = params.params;
  scoped_ptr<PrepareFrameAndViewForPrint> prep_frame_view;
  if (!prepare) {
    prep_frame_view.reset(new PrepareFrameAndViewForPrint(print_params, frame,
                                                          node));
    prepare = prep_frame_view.get();
  }
  UpdatePrintableSizeInPrintParameters(frame, node, prepare, &print_params);

  int page_count = prepare->GetExpectedPageCount();
  if (!page_count)
    return false;
  Send(new PrintHostMsg_DidGetPrintedPagesCount(routing_id(),
                                                print_params.document_cookie,
                                                page_count));

  const gfx::Size& canvas_size = prepare->GetPrintCanvasSize();
  PrintMsg_PrintPage_Params page_params;
  page_params.params = print_params;
  if (params.pages.empty()) {
    for (int i = 0; i < page_count; ++i) {
      page_params.page_number = i;
      PrintPageInternal(page_params, canvas_size, frame);
    }
  } else {
    for (size_t i = 0; i < params.pages.size(); ++i) {
      if (params.pages[i] >= page_count)
        break;
      page_params.page_number = params.pages[i];
      PrintPageInternal(page_params, canvas_size, frame);
    }
  }
  return true;
}
