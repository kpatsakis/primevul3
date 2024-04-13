v8::Handle<v8::Value> V8DOMWrapper::convertEventListenerToV8Object(EventListener* listener)
{
    if (!listener)
        return v8::Null();

    V8AbstractEventListener* v8listener = static_cast<V8AbstractEventListener*>(listener);
    return v8listener->getListenerObject();
}
