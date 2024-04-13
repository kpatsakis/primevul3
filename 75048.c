v8::Handle<v8::Context> V8DOMWrapper::getWrapperContext(Frame* frame)
{
    v8::Handle<v8::Context> context = V8Proxy::context(frame);
    if (context.IsEmpty())
        return v8::Handle<v8::Context>();

    if (V8IsolatedWorld* world = V8IsolatedWorld::getEntered()) {
       context = world->context();
       if (frame != V8Proxy::retrieveFrame(context))
          return v8::Handle<v8::Context>();
    }

    return context;
}
