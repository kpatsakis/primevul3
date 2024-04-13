PassRefPtr<NodeFilter> V8DOMWrapper::wrapNativeNodeFilter(v8::Handle<v8::Value> filter)
{
    if (!filter->IsFunction())
        return 0;

    NodeFilterCondition* condition = new V8NodeFilterCondition(filter);
    return NodeFilter::create(condition);
}
