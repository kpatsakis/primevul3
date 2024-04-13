bool PrintWebViewHelper::InitPrintSettingsAndPrepareFrame(
    WebKit::WebFrame* frame, const WebKit::WebNode& node,
    scoped_ptr<PrepareFrameAndViewForPrint>* prepare) {
  if (!InitPrintSettings(frame, node))
    return false;

  DCHECK(!prepare->get());
  prepare->reset(new PrepareFrameAndViewForPrint(print_pages_params_->params,
                                                 frame, node));
  UpdatePrintableSizeInPrintParameters(frame, node, prepare->get(),
                                       &print_pages_params_->params);
  Send(new PrintHostMsg_DidGetDocumentCookie(
        routing_id(), print_pages_params_->params.document_cookie));
  return true;
}
