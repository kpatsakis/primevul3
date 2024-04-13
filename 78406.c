v8::Local<v8::Context> ScriptController::currentWorldContext()
{
    if (!v8::Context::InContext())
        return contextForWorld(this, mainThreadNormalWorld());

    v8::Handle<v8::Context> context = v8::Context::GetEntered();
    DOMWrapperWorld* isolatedWorld = DOMWrapperWorld::isolatedWorld(context);
    if (!isolatedWorld)
        return contextForWorld(this, mainThreadNormalWorld());

    Frame* frame = toFrameIfNotDetached(context);
    if (!m_frame)
        return v8::Local<v8::Context>();

    if (m_frame == frame)
        return v8::Local<v8::Context>::New(context);

    return contextForWorld(this, isolatedWorld);
}
