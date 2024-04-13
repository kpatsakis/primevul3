void RootWindow::DispatchHeldMouseMove() {
  if (held_mouse_move_.get()) {
    if (!synthesize_mouse_move_)
      DispatchMouseEventImpl(held_mouse_move_.get());
    held_mouse_move_.reset();
  }
}
