bool PrintWebViewHelper::InitPrintSettings(WebKit::WebFrame* frame,
                                           const WebKit::WebNode& node) {
  DCHECK(frame);
  PrintMsg_PrintPages_Params settings;

  Send(new PrintHostMsg_GetDefaultPrintSettings(routing_id(),
                                                &settings.params));
  bool result = true;
  if (PrintMsg_Print_Params_IsEmpty(settings.params)) {
    render_view()->RunModalAlertDialog(
        frame,
        l10n_util::GetStringUTF16(
            IDS_PRINT_PREVIEW_INVALID_PRINTER_SETTINGS));
    result = false;
  }

  if (result &&
      (settings.params.dpi < kMinDpi || settings.params.document_cookie == 0)) {
    NOTREACHED();
    result = false;
  }

  settings.pages.clear();
  print_pages_params_.reset(new PrintMsg_PrintPages_Params(settings));
  return result;
}
