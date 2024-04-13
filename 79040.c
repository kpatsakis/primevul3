  DeterministicMockClientSocketFactory& socket_factory() {
    return *session_deps_.deterministic_socket_factory.get();
  }
