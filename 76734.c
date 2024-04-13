bool RootWindow::DispatchKeyEvent(KeyEvent* event) {
  DispatchHeldMouseMove();
  KeyEvent translated_event(*event);
  if (translated_event.key_code() == ui::VKEY_UNKNOWN)
    return false;
  client::EventClient* client = client::GetEventClient(GetRootWindow());
  if (client && !client->CanProcessEventsWithinSubtree(focused_window_)) {
    SetFocusedWindow(NULL, NULL);
    return false;
  }
  return ProcessKeyEvent(focused_window_, &translated_event);
}
