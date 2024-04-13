v8::Handle<v8::Value> V8DOMWrapper::convertEventTargetToV8Object(EventTarget* target)
{
    if (!target)
        return v8::Null();

#if ENABLE(SVG)
    SVGElementInstance* instance = target->toSVGElementInstance();
    if (instance)
        return convertToV8Object(V8ClassIndex::SVGELEMENTINSTANCE, instance);
#endif

#if ENABLE(WORKERS)
    Worker* worker = target->toWorker();
    if (worker)
        return convertToV8Object(V8ClassIndex::WORKER, worker);
#endif // WORKERS

    Node* node = target->toNode();
    if (node)
        return convertNodeToV8Object(node);

    if (DOMWindow* domWindow = target->toDOMWindow())
        return convertToV8Object(V8ClassIndex::DOMWINDOW, domWindow);

    XMLHttpRequest* xmlHttpRequest = target->toXMLHttpRequest();
    if (xmlHttpRequest) {
        v8::Handle<v8::Object> wrapper = getActiveDOMObjectMap().get(xmlHttpRequest);
        ASSERT(!wrapper.IsEmpty());
        return wrapper;
    }

    MessagePort* port = target->toMessagePort();
    if (port) {
        v8::Handle<v8::Object> wrapper = getActiveDOMObjectMap().get(port);
        ASSERT(!wrapper.IsEmpty());
        return wrapper;
    }

    XMLHttpRequestUpload* upload = target->toXMLHttpRequestUpload();
    if (upload) {
        v8::Handle<v8::Object> wrapper = getDOMObjectMap().get(upload);
        ASSERT(!wrapper.IsEmpty());
        return wrapper;
    }

    ASSERT(0);
    return notHandledByInterceptor();
}
