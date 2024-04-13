v8::Local<v8::Object> V8DOMWrapper::instantiateV8Object(V8Proxy* proxy, V8ClassIndex::V8WrapperType descriptorType, V8ClassIndex::V8WrapperType cptrType, void* impl)
{
    if (descriptorType == V8ClassIndex::HTMLCOLLECTION && static_cast<HTMLCollection*>(impl)->type() == DocAll)
        descriptorType = V8ClassIndex::HTMLALLCOLLECTION;

    if (V8IsolatedWorld::getEntered()) {
        proxy = 0;
    } else if (!proxy)
        proxy = V8Proxy::retrieve();

    v8::Local<v8::Object> instance;
    if (proxy)
        instance = proxy->createWrapperFromCache(descriptorType);
    else {
        v8::Local<v8::Function> function = getTemplate(descriptorType)->GetFunction();
        instance = SafeAllocation::newInstance(function);
    }
    if (!instance.IsEmpty()) {
        setDOMWrapper(instance, V8ClassIndex::ToInt(cptrType), impl);
    }
    return instance;
}
