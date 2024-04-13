 bool ChromotingInstance::IsConnected() {
   return host_connection_.get() &&
     (host_connection_->state() == protocol::ConnectionToHost::CONNECTED);
}
