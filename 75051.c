bool V8DOMWrapper::isDOMEventWrapper(v8::Handle<v8::Value> value)
{
    return isWrapperOfType(value, V8ClassIndex::EVENT);
}
