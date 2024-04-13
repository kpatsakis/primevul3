void ChromotingInstance::SendOutgoingIq(const std::string& iq) {
  scoped_ptr<base::DictionaryValue> data(new base::DictionaryValue());
  data->SetString("iq", iq);
  PostChromotingMessage("sendOutgoingIq", data.Pass());
}
