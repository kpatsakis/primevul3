v8::Local<v8::Context> ScriptController::mainWorldContext(Frame* frame)
{
    if (!frame)
        return v8::Local<v8::Context>();

    return contextForWorld(frame->script(), mainThreadNormalWorld());
}
