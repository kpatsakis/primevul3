LRESULT OmniboxViewWin::OnImeNotify(UINT message,
                                    WPARAM wparam,
                                    LPARAM lparam) {
  switch (wparam) {
    case IMN_OPENCANDIDATE:
      ime_candidate_window_open_ = true;
      ClosePopup();
      break;
    case IMN_CLOSECANDIDATE:
      ime_candidate_window_open_ = false;
      UpdatePopup();
      break;
    default:
      break;
  }
  return DefWindowProc(message, wparam, lparam);
}
