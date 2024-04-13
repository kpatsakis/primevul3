RootWindow::RootWindow(const gfx::Rect& initial_bounds)
    : Window(NULL),
      host_(aura::RootWindowHost::Create(initial_bounds)),
      ALLOW_THIS_IN_INITIALIZER_LIST(schedule_paint_factory_(this)),
      ALLOW_THIS_IN_INITIALIZER_LIST(event_factory_(this)),
      mouse_button_flags_(0),
      last_cursor_(ui::kCursorNull),
      cursor_shown_(true),
      capture_window_(NULL),
      mouse_pressed_handler_(NULL),
      mouse_moved_handler_(NULL),
      focused_window_(NULL),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          gesture_recognizer_(ui::GestureRecognizer::Create(this))),
      synthesize_mouse_move_(false),
      waiting_on_compositing_end_(false),
      draw_on_compositing_end_(false),
      defer_draw_scheduling_(false),
      mouse_move_hold_count_(0),
      should_hold_mouse_moves_(false),
      compositor_lock_(NULL),
      draw_on_compositor_unlock_(false),
      draw_trace_count_(0) {
  SetName("RootWindow");
  should_hold_mouse_moves_ = !CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kAuraDisableHoldMouseMoves);

  compositor_.reset(new ui::Compositor(this, host_->GetAcceleratedWidget()));
  DCHECK(compositor_.get());
  compositor_->AddObserver(this);
}
