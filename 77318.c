int OmniboxViewWin::OnPerformDrop(const views::DropTargetEvent& event) {
  return OnPerformDropImpl(event, false);
}
