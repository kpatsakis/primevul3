void ExecuteCodeInTabFunction::OnExecuteCodeFinished(const std::string& error,
                                                     int32 on_page_id,
                                                     const GURL& on_url,
                                                     const ListValue& result) {
  if (!error.empty())
    SetError(error);

  SendResponse(error.empty());
}
