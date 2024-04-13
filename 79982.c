void RenderViewImpl::EvaluateScript(const string16& frame_xpath,
                                    const string16& jscript,
                                    int id,
                                    bool notify_result) {
  v8::HandleScope handle_scope;
  v8::Handle<v8::Value> result;
  WebFrame* web_frame = GetChildFrame(frame_xpath);
  if (web_frame)
    result = web_frame->executeScriptAndReturnValue(WebScriptSource(jscript));
  if (notify_result) {
    base::ListValue list;
    if (!result.IsEmpty() && web_frame) {
      v8::Local<v8::Context> context = web_frame->mainWorldScriptContext();
      v8::Context::Scope context_scope(context);
      V8ValueConverterImpl converter;
      converter.SetDateAllowed(true);
      converter.SetRegExpAllowed(true);
      base::Value* result_value = converter.FromV8Value(result, context);
      list.Set(0, result_value ? result_value : base::Value::CreateNullValue());
    } else {
      list.Set(0, base::Value::CreateNullValue());
    }
    Send(new ViewHostMsg_ScriptEvalResponse(routing_id_, id, list));
  }
}
