v8::Local<v8::Value> ScriptController::callFunction(v8::Handle<v8::Function> function, v8::Handle<v8::Object> receiver, int argc, v8::Handle<v8::Value> args[])
{
    RefPtr<Frame> protect(m_frame);
    return ScriptController::callFunctionWithInstrumentation(m_frame ? m_frame->document() : 0, function, receiver, argc, args);
}
