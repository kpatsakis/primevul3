bool OmniboxViewWin::IsImeComposing() const {
  bool ime_composing = false;
  HIMC context = ImmGetContext(m_hWnd);
  if (context) {
    ime_composing = !!ImmGetCompositionString(context, GCS_COMPSTR, NULL, 0);
    ImmReleaseContext(m_hWnd, context);
  }
  return ime_composing;
}
