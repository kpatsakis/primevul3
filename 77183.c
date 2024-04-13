DOMWrapperMap<void>& getDOMObjectMap(v8::Isolate* isolate)
{
    return DOMData::getCurrentStore(isolate).domObjectMap();
}
