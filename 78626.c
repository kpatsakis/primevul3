 void ColorChooserDialog::CopyCustomColors(COLORREF* src, COLORREF* dst) {
  memcpy(dst, src, sizeof(COLORREF) * arraysize(g_custom_colors));
}
