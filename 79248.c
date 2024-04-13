bool WindowsGetLastFocusedFunction::RunImpl() {
  scoped_ptr<GetLastFocused::Params> params(
      GetLastFocused::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  bool populate_tabs = false;
  if (params->get_info.get() && params->get_info->populate.get())
    populate_tabs = *params->get_info->populate;

  Browser* browser = chrome::FindAnyBrowser(
      profile(), include_incognito(), chrome::GetActiveDesktop());
  if (!browser || !browser->window()) {
    error_ = keys::kNoLastFocusedWindowError;
    return false;
  }
  WindowController* controller =
      browser->extension_window_controller();
  if (populate_tabs)
    SetResult(controller->CreateWindowValueWithTabs(GetExtension()));
  else
    SetResult(controller->CreateWindowValue());
  return true;
}
