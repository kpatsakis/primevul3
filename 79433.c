inline v8::Handle<v8::Value> DialogHandler::returnValue() const
{
    if (m_dialogContext.IsEmpty())
        return v8::Undefined();
    v8::Context::Scope scope(m_dialogContext);
    v8::Handle<v8::Value> returnValue = m_dialogContext->Global()->Get(v8::String::NewSymbol("returnValue"));
    if (returnValue.IsEmpty())
        return v8::Undefined();
    return returnValue;
}
