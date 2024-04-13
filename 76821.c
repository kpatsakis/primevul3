gfx::Size RootWindowHost::GetNativeScreenSize() {
  return gfx::Size(GetSystemMetrics(SM_CXSCREEN),
                   GetSystemMetrics(SM_CYSCREEN));
}
