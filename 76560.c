void ChromotingInstance::OnIncomingIq(const std::string& iq) {
  xmpp_proxy_->OnIq(iq);
}
