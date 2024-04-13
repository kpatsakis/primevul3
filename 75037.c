v8::Handle<v8::Value> V8DOMWrapper::convertNodeToV8Object(Node* node)
{
    if (!node)
        return v8::Null();

    V8Proxy* proxy = 0;
    Document* document = node->document();
    if (document) {
        Frame* frame = document->frame();
        proxy = V8Proxy::retrieve(frame);
        if (proxy)
            proxy->initContextIfNeeded();
    }

    DOMWrapperMap<Node>& domNodeMap = getDOMNodeMap();
    v8::Handle<v8::Object> wrapper = domNodeMap.get(node);
    if (!wrapper.IsEmpty())
        return wrapper;

    bool isDocument = false; // document type node has special handling
    V8ClassIndex::V8WrapperType type;

    Node::NodeType nodeType = node->nodeType();
    if (nodeType == Node::ELEMENT_NODE) {
        if (node->isHTMLElement())
            type = htmlElementType(static_cast<HTMLElement*>(node));
#if ENABLE(SVG)
        else if (node->isSVGElement())
            type = svgElementType(static_cast<SVGElement*>(node));
#endif
        else
            type = V8ClassIndex::ELEMENT;
    } else if (nodeType == Node::DOCUMENT_NODE) {
        isDocument = true;
        Document* document = static_cast<Document*>(node);
        if (document->isHTMLDocument())
            type = V8ClassIndex::HTMLDOCUMENT;
#if ENABLE(SVG)
        else if (document->isSVGDocument())
            type = V8ClassIndex::SVGDOCUMENT;
#endif
        else
            type = V8ClassIndex::DOCUMENT;
    } else {
        ASSERT(nodeType < sizeof(mapping)/sizeof(mapping[0]));
        type = mapping[nodeType];
        ASSERT(type != V8ClassIndex::INVALID_CLASS_INDEX);
    }

    v8::Handle<v8::Context> context;
    if (proxy)
        context = getWrapperContext(proxy->frame());

    if (!context.IsEmpty())
        context->Enter();

    v8::Local<v8::Object> result = instantiateV8Object(proxy, type, V8ClassIndex::NODE, node);

    if (!context.IsEmpty())
        context->Exit();

    if (result.IsEmpty()) {
        return result;
    }

    node->ref();
    domNodeMap.set(node, v8::Persistent<v8::Object>::New(result));

    if (isDocument) {
        if (proxy)
            proxy->updateDocumentWrapper(result);

        if (type == V8ClassIndex::HTMLDOCUMENT) {
            ASSERT(result->InternalFieldCount() == V8Custom::kHTMLDocumentInternalFieldCount);
            v8::Local<v8::Object> marker = v8::Object::New();
            result->SetInternalField(V8Custom::kHTMLDocumentMarkerIndex, marker);
            result->SetInternalField(V8Custom::kHTMLDocumentShadowIndex, marker);
        }
    }

    return result;
}
