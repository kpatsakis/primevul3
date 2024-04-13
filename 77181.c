DOMWrapperMap<void>& getActiveDOMObjectMap(v8::Isolate* isolate)
{
    return DOMData::getCurrentStore(isolate).activeDomObjectMap();
}
