DOMNodeMapping& getDOMNodeMap(v8::Isolate* isolate)
{
    return DOMData::getCurrentStore(isolate).domNodeMap();
}
