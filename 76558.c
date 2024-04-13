void ChromotingInstance::OnConnectionState(
    protocol::ConnectionToHost::State state,
    protocol::ErrorCode error) {
  scoped_ptr<base::DictionaryValue> data(new base::DictionaryValue());
  data->SetString("state", ConnectionStateToString(state));
  data->SetString("error", ConnectionErrorToString(error));
  PostChromotingMessage("onConnectionStatus", data.Pass());
}
