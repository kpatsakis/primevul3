void V8DOMWrapper::setJSWrapperForDOMNode(Node* node, v8::Persistent<v8::Object> wrapper)
{
    ASSERT(V8DOMWrapper::maybeDOMWrapper(wrapper));
    getDOMNodeMap().set(node, wrapper);
}
