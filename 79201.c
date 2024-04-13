void DevToolsWindow::ScheduleAction(const DevToolsToggleAction& action) {
  action_on_load_ = action;
  if (is_loaded_)
    DoAction();
}
