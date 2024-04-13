void DevToolsWindow::DispatchOnEmbedder(const std::string& message) {
  std::string method;
  base::ListValue empty_params;
  base::ListValue* params = &empty_params;

  base::DictionaryValue* dict = NULL;
  scoped_ptr<base::Value> parsed_message(base::JSONReader::Read(message));
  if (!parsed_message ||
      !parsed_message->GetAsDictionary(&dict) ||
      !dict->GetString(kFrontendHostMethod, &method) ||
      (dict->HasKey(kFrontendHostParams) &&
          !dict->GetList(kFrontendHostParams, &params))) {
    LOG(ERROR) << "Invalid message was sent to embedder: " << message;
    return;
  }

  int id = 0;
  dict->GetInteger(kFrontendHostId, &id);

  std::string error = embedder_message_dispatcher_->Dispatch(method, params);
  if (id) {
    scoped_ptr<base::Value> id_value(base::Value::CreateIntegerValue(id));
    scoped_ptr<base::Value> error_value(base::Value::CreateStringValue(error));
    CallClientFunction("InspectorFrontendAPI.embedderMessageAck",
                       id_value.get(), error_value.get(), NULL);
  }
}
