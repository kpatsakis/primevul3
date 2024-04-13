void HTMLTextAreaElement::setDefaultValue(const String& defaultValue)
{
    RefPtr<Node> protectFromMutationEvents(this);

    Vector<RefPtr<Node> > textNodes;
    for (Node* n = firstChild(); n; n = n->nextSibling()) {
        if (n->isTextNode())
            textNodes.append(n);
    }
    size_t size = textNodes.size();
    for (size_t i = 0; i < size; ++i)
        removeChild(textNodes[i].get(), IGNORE_EXCEPTION);

    String value = defaultValue;
    value.replace("\r\n", "\n");
    value.replace('\r', '\n');

    insertBefore(document().createTextNode(value), firstChild(), IGNORE_EXCEPTION);

    if (!m_isDirty)
        setNonDirtyValue(value);
}
