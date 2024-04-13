void BrowserTitlebar::Throbber::InitFrames() {
  if (g_throbber_frames)
    return;

  g_throbber_frames = new std::vector<GdkPixbuf*>;
  MakeThrobberFrames(IDR_THROBBER_LIGHT, g_throbber_frames);

  g_throbber_waiting_frames = new std::vector<GdkPixbuf*>;
  MakeThrobberFrames(IDR_THROBBER_WAITING_LIGHT, g_throbber_waiting_frames);
}
