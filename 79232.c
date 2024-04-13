bool TabsCaptureVisibleTabFunction::GetTabToCapture(
    WebContents** web_contents) {
  Browser* browser = NULL;
  int window_id = extension_misc::kCurrentWindowId;

  if (HasOptionalArgument(0))
    EXTENSION_FUNCTION_VALIDATE(args_->GetInteger(0, &window_id));

  if (!GetBrowserFromWindowID(this, window_id, &browser))
    return false;

  *web_contents = browser->tab_strip_model()->GetActiveWebContents();
  if (*web_contents == NULL) {
    error_ = keys::kInternalVisibleTabCaptureError;
    return false;
  }

  return true;
};
