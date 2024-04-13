bool RootWindow::DispatchMouseEventToTarget(MouseEvent* event,
                                            Window* target) {
  static const int kMouseButtonFlagMask =
      ui::EF_LEFT_MOUSE_BUTTON |
      ui::EF_MIDDLE_MOUSE_BUTTON |
      ui::EF_RIGHT_MOUSE_BUTTON;
  last_mouse_location_ = event->location();
  synthesize_mouse_move_ = false;
  switch (event->type()) {
    case ui::ET_MOUSE_MOVED:
      HandleMouseMoved(*event, target);
      break;
    case ui::ET_MOUSE_PRESSED:
      if (!mouse_pressed_handler_)
        mouse_pressed_handler_ = target;
      mouse_button_flags_ = event->flags() & kMouseButtonFlagMask;
      Env::GetInstance()->set_mouse_button_flags(mouse_button_flags_);
      break;
    case ui::ET_MOUSE_RELEASED:
      mouse_pressed_handler_ = NULL;
      mouse_button_flags_ = event->flags() & kMouseButtonFlagMask;
      Env::GetInstance()->set_mouse_button_flags(mouse_button_flags_);
      break;
    default:
      break;
  }
  if (target && target->delegate()) {
    int flags = event->flags();
    gfx::Point location_in_window = event->location();
    Window::ConvertPointToWindow(this, target, &location_in_window);
    if (IsNonClientLocation(target, location_in_window))
      flags |= ui::EF_IS_NON_CLIENT;
    MouseEvent translated_event(*event, this, target, event->type(), flags);
    return ProcessMouseEvent(target, &translated_event);
  }
  return false;
}
