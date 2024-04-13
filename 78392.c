v8::Local<v8::Value> ScriptController::callFunctionWithInstrumentation(ScriptExecutionContext* context, v8::Handle<v8::Function> function, v8::Handle<v8::Object> receiver, int argc, v8::Handle<v8::Value> args[])
{
    InspectorInstrumentationCookie cookie;
    if (InspectorInstrumentation::timelineAgentEnabled(context)) {
        String resourceName;
        int lineNumber;
        resourceInfo(function, resourceName, lineNumber);
        cookie = InspectorInstrumentation::willCallFunction(context, resourceName, lineNumber);
    }

    v8::Local<v8::Value> result = V8ScriptRunner::callFunction(function, context, receiver, argc, args);

    InspectorInstrumentation::didCallFunction(cookie);
    return result;
}
