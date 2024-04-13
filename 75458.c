const gfx::Font& BrowserFrame::GetTitleFont() {
  static gfx::Font *title_font = new gfx::Font();
  return *title_font;
}
