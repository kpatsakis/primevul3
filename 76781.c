void RootWindow::SynthesizeMouseMoveEvent() {
  if (!synthesize_mouse_move_)
    return;
  synthesize_mouse_move_ = false;
#if !defined(OS_WIN)
  gfx::Point orig_mouse_location = last_mouse_location_;
  layer()->transform().TransformPoint(orig_mouse_location);

  MouseEvent event(ui::ET_MOUSE_MOVED,
                   orig_mouse_location,
                   orig_mouse_location,
                   ui::EF_IS_SYNTHESIZED);
  DispatchMouseEvent(&event);
#endif
}
