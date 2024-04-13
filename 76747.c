void RootWindow::HandleMouseMoved(const MouseEvent& event, Window* target) {
  if (target == mouse_moved_handler_)
    return;

  if (mouse_moved_handler_ && mouse_moved_handler_->delegate()) {
    MouseEvent translated_event(event, this, mouse_moved_handler_,
                                ui::ET_MOUSE_EXITED, event.flags());
    ProcessMouseEvent(mouse_moved_handler_, &translated_event);
  }
  mouse_moved_handler_ = target;
  if (mouse_moved_handler_ && mouse_moved_handler_->delegate()) {
    MouseEvent translated_event(event, this, mouse_moved_handler_,
                                ui::ET_MOUSE_ENTERED, event.flags());
    ProcessMouseEvent(mouse_moved_handler_, &translated_event);
  }
}
