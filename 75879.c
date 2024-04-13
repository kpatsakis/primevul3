  void ReopenDatabase() {
    session_helper_.set_service(NULL);
    profile()->set_session_service(NULL);

    SessionService* service = new SessionService(profile());
    profile()->set_session_service(service);
    session_helper_.set_service(service);
  }
