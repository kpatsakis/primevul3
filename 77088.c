DesktopNativeWidgetHelperAura::~DesktopNativeWidgetHelperAura() {
  if (root_window_event_filter_) {
#if defined(USE_X11)
    root_window_event_filter_->RemoveFilter(x11_window_event_filter_.get());
#endif

    root_window_event_filter_->RemoveFilter(input_method_filter_.get());
  }
}
