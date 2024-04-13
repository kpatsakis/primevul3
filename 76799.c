bool RootWindowHostLinux::IsWindowManagerPresent() {
  return XGetSelectionOwner(
      xdisplay_,
      X11AtomCache::GetInstance()->GetAtom(ui::ATOM_WM_S0)) != None;
}
