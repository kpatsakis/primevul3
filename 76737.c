void RootWindow::Draw() {
  if (waiting_on_compositing_end_) {
    draw_on_compositing_end_ = true;
    defer_draw_scheduling_ = false;
    return;
  }
  if (compositor_lock_) {
    draw_on_compositor_unlock_ = true;
    defer_draw_scheduling_ = false;
    return;
  }
  waiting_on_compositing_end_ = true;

  TRACE_EVENT_ASYNC_BEGIN0("ui", "RootWindow::Draw", draw_trace_count_++);

  compositor_->Draw(false);
  defer_draw_scheduling_ = false;
}
