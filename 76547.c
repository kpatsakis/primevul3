 std::string ConnectionStateToString(protocol::ConnectionToHost::State state) {
  switch (state) {
    case protocol::ConnectionToHost::INITIALIZING:
      return "INITIALIZING";
    case protocol::ConnectionToHost::CONNECTING:
      return "CONNECTING";
    case protocol::ConnectionToHost::CONNECTED:
      return "CONNECTED";
    case protocol::ConnectionToHost::CLOSED:
      return "CLOSED";
    case protocol::ConnectionToHost::FAILED:
      return "FAILED";
  }
  NOTREACHED();
  return "";
}
