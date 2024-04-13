v8::Handle<v8::Value> V8DOMWrapper::convertStyleSheetToV8Object(StyleSheet* sheet)
{
    if (!sheet)
        return v8::Null();

    v8::Handle<v8::Object> wrapper = getDOMObjectMap().get(sheet);
    if (!wrapper.IsEmpty())
        return wrapper;

    V8ClassIndex::V8WrapperType type = V8ClassIndex::STYLESHEET;
    if (sheet->isCSSStyleSheet())
        type = V8ClassIndex::CSSSTYLESHEET;

    v8::Handle<v8::Object> result = instantiateV8Object(type, V8ClassIndex::STYLESHEET, sheet);
    if (!result.IsEmpty()) {
        sheet->ref();
        setJSWrapperForDOMObject(sheet, v8::Persistent<v8::Object>::New(result));
    }

    Node* ownerNode = sheet->ownerNode();
    if (ownerNode) {
        v8::Handle<v8::Object> owner = v8::Handle<v8::Object>::Cast(convertNodeToV8Object(ownerNode));
        result->SetInternalField(V8Custom::kStyleSheetOwnerNodeIndex, owner);
    }

    return result;
}
