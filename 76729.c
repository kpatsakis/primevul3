ui::GestureEvent* RootWindow::CreateGestureEvent(ui::EventType type,
    const gfx::Point& location,
    int flags,
    const base::Time time,
    float param_first,
    float param_second,
    unsigned int touch_id_bitfield) {
  return new GestureEvent(type, location.x(), location.y(), flags, time,
                          param_first, param_second, touch_id_bitfield);
}
