void RootWindow::ScheduleDraw() {
  if (compositor_lock_) {
    draw_on_compositor_unlock_ = true;
  } else if (!defer_draw_scheduling_) {
    defer_draw_scheduling_ = true;
    MessageLoop::current()->PostTask(
        FROM_HERE,
        base::Bind(&RootWindow::Draw, schedule_paint_factory_.GetWeakPtr()));
  }
}
