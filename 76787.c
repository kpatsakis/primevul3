RootWindowHost* RootWindowHost::Create(const gfx::Rect& bounds) {
  return new RootWindowHostLinux(bounds);
}
