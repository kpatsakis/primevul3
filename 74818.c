void RenderView::OnPrintingDone(int document_cookie, bool success) {
  DCHECK(print_helper_.get());
  if (print_helper_.get() != NULL) {
    print_helper_->DidFinishPrinting(success);
  }
}
