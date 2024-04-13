void PrintWebViewHelper::OnPrintForPrintPreview(
    const DictionaryValue& job_settings) {
  DCHECK(is_preview_enabled_);
  if (print_web_view_)
    return;

  if (!render_view()->GetWebView())
    return;
  WebFrame* main_frame = render_view()->GetWebView()->mainFrame();
  if (!main_frame)
    return;

  WebDocument document = main_frame->document();
  WebElement pdf_element = document.getElementById("pdf-viewer");
  if (pdf_element.isNull()) {
    NOTREACHED();
    return;
  }

  WebFrame* pdf_frame = pdf_element.document().frame();
  if (!UpdatePrintSettings(pdf_frame, pdf_element, job_settings, true)) {
    LOG(ERROR) << "UpdatePrintSettings failed";
    DidFinishPrinting(FAIL_PRINT);
    return;
  }

  scoped_ptr<PrepareFrameAndViewForPrint> prepare;
  prepare.reset(new PrepareFrameAndViewForPrint(print_pages_params_->params,
                                                pdf_frame, pdf_element));
  UpdatePrintableSizeInPrintParameters(pdf_frame, pdf_element, prepare.get(),
                                       &print_pages_params_->params);

  if (!RenderPagesForPrint(pdf_frame, pdf_element, prepare.get())) {
    LOG(ERROR) << "RenderPagesForPrint failed";
    DidFinishPrinting(FAIL_PRINT);
  }
}
