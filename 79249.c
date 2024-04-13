bool WindowsGetAllFunction::RunImpl() {
  scoped_ptr<GetAll::Params> params(GetAll::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  bool populate_tabs = false;
  if (params->get_info.get() && params->get_info->populate.get())
    populate_tabs = *params->get_info->populate;

  ListValue* window_list = new ListValue();
  const WindowControllerList::ControllerList& windows =
      WindowControllerList::GetInstance()->windows();
  for (WindowControllerList::ControllerList::const_iterator iter =
           windows.begin();
       iter != windows.end(); ++iter) {
    if (!this->CanOperateOnWindow(*iter))
      continue;
    if (populate_tabs)
      window_list->Append((*iter)->CreateWindowValueWithTabs(GetExtension()));
    else
      window_list->Append((*iter)->CreateWindowValue());
  }
  SetResult(window_list);
  return true;
}
