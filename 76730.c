ui::TouchEvent* RootWindow::CreateTouchEvent(ui::EventType type,
                                             const gfx::Point& location,
                                             int touch_id,
                                             base::TimeDelta time_stamp) {
  return new TouchEvent(type, location, touch_id, time_stamp);
}
