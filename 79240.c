void TabsUpdateFunction::OnExecuteCodeFinished(const std::string& error,
                                              int32 on_page_id,
                                              const GURL& url,
                                              const ListValue& script_result) {
  if (error.empty())
    PopulateResult();
  else
    error_ = error;
  SendResponse(error.empty());
}
