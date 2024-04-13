  void GetLastSession() {
    profile()->GetSessionService()->TabClosed(controller().window_id(),
                                              controller().session_id(),
                                              false);

    ReopenDatabase();
    Time close_time;

    session_helper_.ReadWindows(&windows_);
  }
