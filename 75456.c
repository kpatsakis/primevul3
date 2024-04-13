BrowserFrameGtk::BrowserFrameGtk(BrowserFrame* browser_frame,
                                 BrowserView* browser_view)
    : views::NativeWidgetGtk(browser_frame),
      browser_view_(browser_view) {
  set_focus_on_creation(false);
}
