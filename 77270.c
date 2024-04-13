gfx::NativeView OmniboxViewWin::GetRelativeWindowForNativeView(
    gfx::NativeView edit_native_view) {
  HWND ime_window = ImmGetDefaultIMEWnd(edit_native_view);
  return ime_window ? ime_window : HWND_NOTOPMOST;
}
