void RenderView::Print(WebFrame* frame, bool script_initiated) {
  DCHECK(frame);
  if (print_helper_.get() == NULL) {
    print_helper_.reset(new PrintWebViewHelper(this));
  }
  print_helper_->Print(frame, script_initiated);
}
