void PrintWebViewHelper::RequestPrintPreview() {
  old_print_pages_params_.reset();
  Send(new PrintHostMsg_RequestPrintPreview(routing_id()));
}
