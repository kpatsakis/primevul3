void PrintWebViewHelper::didStopLoading() {
  PrintMsg_PrintPages_Params* params = print_pages_params_.get();
  DCHECK(params != NULL);
  PrintPages(*params, print_web_view_->mainFrame(), WebNode(), NULL);
}
