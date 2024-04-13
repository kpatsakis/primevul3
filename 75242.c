void CSSStyleSheet::addNamespace(CSSParser* p, const AtomicString& prefix, const AtomicString& uri)
{
    if (uri.isNull())
        return;

    m_namespaces = new CSSNamespace(prefix, uri, m_namespaces);
    
    if (prefix.isEmpty())
        p->m_defaultNamespace = uri;
}
