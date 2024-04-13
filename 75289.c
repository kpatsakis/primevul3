void PrintWebViewHelper::DisplayPrintJobError() {
  WebView* web_view = print_web_view_;
  if (!web_view)
    web_view = render_view()->GetWebView();

  render_view()->RunModalAlertDialog(
      web_view->mainFrame(),
      l10n_util::GetStringUTF16(IDS_PRINT_SPOOL_FAILED_ERROR_TEXT));
}
