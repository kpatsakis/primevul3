void RootWindow::ReleaseMouseMoves() {
  if (should_hold_mouse_moves_) {
    --mouse_move_hold_count_;
    DCHECK_GE(mouse_move_hold_count_, 0);
    if (!mouse_move_hold_count_)
      DispatchHeldMouseMove();
  }
}
