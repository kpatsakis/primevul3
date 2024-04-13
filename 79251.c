bool WindowsRemoveFunction::RunImpl() {
  int window_id = -1;
  EXTENSION_FUNCTION_VALIDATE(args_->GetInteger(0, &window_id));

  WindowController* controller;
  if (!GetWindowFromWindowID(this, window_id, &controller))
    return false;

#if defined(OS_WIN)
  if (win8::IsSingleWindowMetroMode())
    return false;
#endif

  WindowController::Reason reason;
  if (!controller->CanClose(&reason)) {
    if (reason == WindowController::REASON_NOT_EDITABLE)
      error_ = keys::kTabStripNotEditableError;
    return false;
  }
  controller->window()->Close();
  return true;
}
