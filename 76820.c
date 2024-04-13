RootWindowHost* RootWindowHost::GetForAcceleratedWidget(
    gfx::AcceleratedWidget accelerated_widget) {
  return reinterpret_cast<RootWindowHost*>(
      ui::ViewProp::GetValue(accelerated_widget, kRootWindowHostWinKey));
}
