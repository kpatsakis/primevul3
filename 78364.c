void RenderThreadImpl::OnUpdateScrollbarTheme(
    float initial_button_delay,
    float autoscroll_button_delay,
    bool jump_on_track_click,
    blink::ScrollerStyle preferred_scroller_style,
    bool redraw) {
  EnsureWebKitInitialized();
  static_cast<WebScrollbarBehaviorImpl*>(
      webkit_platform_support_->scrollbarBehavior())->set_jump_on_track_click(
          jump_on_track_click);
  blink::WebScrollbarTheme::updateScrollbars(initial_button_delay,
                                             autoscroll_button_delay,
                                             preferred_scroller_style,
                                             redraw);
}
