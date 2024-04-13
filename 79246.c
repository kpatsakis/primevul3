bool WindowsGetFunction::RunImpl() {
  scoped_ptr<Get::Params> params(Get::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  bool populate_tabs = false;
  if (params->get_info.get() && params->get_info->populate.get())
    populate_tabs = *params->get_info->populate;

  WindowController* controller;
  if (!GetWindowFromWindowID(this, params->window_id, &controller))
    return false;

  if (populate_tabs)
    SetResult(controller->CreateWindowValueWithTabs(GetExtension()));
  else
    SetResult(controller->CreateWindowValue());
  return true;
}
