void RootWindow::HoldMouseMoves() {
  if (should_hold_mouse_moves_)
    ++mouse_move_hold_count_;
}
