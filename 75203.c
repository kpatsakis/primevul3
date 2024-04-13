GdkPixbuf* BrowserTitlebar::Throbber::GetNextFrame(bool is_waiting) {
  Throbber::InitFrames();
  if (is_waiting) {
    return (*g_throbber_waiting_frames)[current_waiting_frame_++ %
        g_throbber_waiting_frames->size()];
  } else {
    return (*g_throbber_frames)[current_frame_++ % g_throbber_frames->size()];
  }
}
