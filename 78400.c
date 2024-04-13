v8::Local<v8::Value> ScriptController::compileAndRunScript(const ScriptSourceCode& source, AccessControlStatus corsStatus)
{
    ASSERT(v8::Context::InContext());

    InspectorInstrumentationCookie cookie = InspectorInstrumentation::willEvaluateScript(m_frame, source.url().isNull() ? String() : source.url().string(), source.startLine());

    v8::Local<v8::Value> result;
    {
        v8::TryCatch tryCatch;
        tryCatch.SetVerbose(true);

        v8::Handle<v8::String> code = v8String(source.source(), m_isolate);
        OwnPtr<v8::ScriptData> scriptData = V8ScriptRunner::precompileScript(code, source.cachedScript());

        v8::Handle<v8::Script> script = V8ScriptRunner::compileScript(code, source.url(), source.startPosition(), scriptData.get(), m_isolate, corsStatus);

        RefPtr<Frame> protect(m_frame);
        result = V8ScriptRunner::runCompiledScript(script, m_frame->document());
        ASSERT(!tryCatch.HasCaught() || result.IsEmpty());
    }

    InspectorInstrumentation::didEvaluateScript(cookie);

    return result;
}
