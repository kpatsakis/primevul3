gfx::Size RootWindowHost::GetNativeScreenSize() {
  ::Display* xdisplay = base::MessagePumpX::GetDefaultXDisplay();
  return gfx::Size(DisplayWidth(xdisplay, 0), DisplayHeight(xdisplay, 0));
}
