void RenderFrameImpl::OnJavaScriptExecuteRequest(
    const base::string16& jscript,
    int id,
    bool notify_result) {
  TRACE_EVENT_INSTANT0("test_tracing", "OnJavaScriptExecuteRequest",
                       TRACE_EVENT_SCOPE_THREAD);

  v8::HandleScope handle_scope(v8::Isolate::GetCurrent());
  v8::Handle<v8::Value> result =
      frame_->executeScriptAndReturnValue(WebScriptSource(jscript));
  if (notify_result) {
    base::ListValue list;
    if (!result.IsEmpty()) {
      v8::Local<v8::Context> context = frame_->mainWorldScriptContext();
      v8::Context::Scope context_scope(context);
      V8ValueConverterImpl converter;
      converter.SetDateAllowed(true);
      converter.SetRegExpAllowed(true);
      base::Value* result_value = converter.FromV8Value(result, context);
      list.Set(0, result_value ? result_value : base::Value::CreateNullValue());
    } else {
      list.Set(0, base::Value::CreateNullValue());
    }
    Send(new FrameHostMsg_JavaScriptExecuteResponse(routing_id_, id, list));
  }
}
