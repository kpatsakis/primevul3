void TabsExecuteScriptFunction::OnExecuteCodeFinished(const std::string& error,
                                                      int32 on_page_id,
                                                      const GURL& on_url,
                                                      const ListValue& result) {
  if (error.empty())
    SetResult(result.DeepCopy());
  ExecuteCodeInTabFunction::OnExecuteCodeFinished(error, on_page_id, on_url,
                                                  result);
}
