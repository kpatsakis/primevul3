  virtual void SetUp() {
    NavigationControllerTest::SetUp();

    SessionService* service = new SessionService(profile());
    profile()->set_session_service(service);
    service->SetWindowType(window_id, Browser::TYPE_NORMAL);
    service->SetWindowBounds(window_id, gfx::Rect(0, 1, 2, 3), false);
    service->SetTabIndexInWindow(window_id,
                                 controller().session_id(), 0);
    controller().SetWindowID(window_id);

    session_helper_.set_service(service);
  }
