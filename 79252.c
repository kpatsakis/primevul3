bool TabsGetSelectedFunction::RunImpl() {
  int window_id = extension_misc::kCurrentWindowId;

  if (HasOptionalArgument(0))
    EXTENSION_FUNCTION_VALIDATE(args_->GetInteger(0, &window_id));

  Browser* browser = NULL;
  if (!GetBrowserFromWindowID(this, window_id, &browser))
    return false;

  TabStripModel* tab_strip = browser->tab_strip_model();
  WebContents* contents = tab_strip->GetActiveWebContents();
  if (!contents) {
    error_ = keys::kNoSelectedTabError;
    return false;
  }
  SetResult(ExtensionTabUtil::CreateTabValue(contents,
                                             tab_strip,
                                             tab_strip->active_index(),
                                             GetExtension()));
  return true;
}
