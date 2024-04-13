DesktopNativeWidgetHelperAura::DesktopNativeWidgetHelperAura(
    NativeWidgetAura* widget)
    : widget_(widget),
      root_window_event_filter_(NULL),
      is_embedded_window_(false) {
}
