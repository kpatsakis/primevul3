void RootWindow::UnlockCompositor() {
  DCHECK(compositor_lock_);
  compositor_lock_ = NULL;
  if (draw_on_compositor_unlock_) {
    draw_on_compositor_unlock_ = false;
    ScheduleDraw();
  }
}
