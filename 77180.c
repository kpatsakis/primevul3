DOMNodeMapping& getActiveDOMNodeMap(v8::Isolate* isolate)
{
    return DOMData::getCurrentStore(isolate).activeDomNodeMap();
}
