const AtomicString& CSSStyleSheet::determineNamespace(const AtomicString& prefix)
{
    if (prefix.isNull())
        return nullAtom; // No namespace. If an element/attribute has a namespace, we won't match it.
    if (prefix == starAtom)
        return starAtom; // We'll match any namespace.
    if (m_namespaces) {
        CSSNamespace* ns = m_namespaces->namespaceForPrefix(prefix);
        if (ns)
            return ns->uri();
    }
    return nullAtom; // Assume we wont match any namespaces.
}
