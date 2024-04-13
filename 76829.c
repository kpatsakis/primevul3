void RootWindowHostWin::PostNativeEvent(const base::NativeEvent& native_event) {
  ::PostMessage(
      hwnd(), native_event.message, native_event.wParam, native_event.lParam);
}
