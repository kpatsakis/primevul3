RootWindowHost* RootWindowHost::Create(const gfx::Rect& bounds) {
  return new RootWindowHostWin(bounds);
}
