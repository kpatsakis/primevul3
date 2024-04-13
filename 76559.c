void ChromotingInstance::OnFirstFrameReceived() {
  scoped_ptr<base::DictionaryValue> data(new base::DictionaryValue());
  PostChromotingMessage("onFirstFrameReceived", data.Pass());
}
