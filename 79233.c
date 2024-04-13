bool GetWindowFromWindowID(UIThreadExtensionFunction* function,
                           int window_id,
                           WindowController** controller) {
  if (window_id == extension_misc::kCurrentWindowId) {
    WindowController* extension_window_controller =
        function->dispatcher()->delegate()->GetExtensionWindowController();
    if (extension_window_controller) {
      *controller = extension_window_controller;
    } else {
      *controller = WindowControllerList::GetInstance()->
          CurrentWindowForFunction(function);
    }
    if (!(*controller)) {
      function->SetError(keys::kNoCurrentWindowError);
      return false;
    }
  } else {
    *controller = WindowControllerList::GetInstance()->
        FindWindowForFunctionById(function, window_id);
    if (!(*controller)) {
      function->SetError(ErrorUtils::FormatErrorMessage(
          keys::kWindowNotFoundError, base::IntToString(window_id)));
      return false;
    }
  }
  return true;
}
