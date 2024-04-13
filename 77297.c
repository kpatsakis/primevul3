void OmniboxViewWin::OnCut() {
  OnCopy();

  ReplaceSel(L"", true);
}
