void PrintWebViewHelper::OnInitiatePrintPreview() {
  DCHECK(is_preview_enabled_);
  WebFrame* frame;
  if (GetPrintFrame(&frame)) {
    print_preview_context_.InitWithFrame(frame);
    RequestPrintPreview();
  }
}
