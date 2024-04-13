void XMLTokenizer::startElementNs(const xmlChar* xmlLocalName, const xmlChar* xmlPrefix, const xmlChar* xmlURI, int nb_namespaces,
                                  const xmlChar** libxmlNamespaces, int nb_attributes, int nb_defaulted, const xmlChar** libxmlAttributes)
{
    if (m_parserStopped)
        return;
    
    if (m_parserPaused) {
        m_pendingCallbacks->appendStartElementNSCallback(xmlLocalName, xmlPrefix, xmlURI, nb_namespaces, libxmlNamespaces,
                                                         nb_attributes, nb_defaulted, libxmlAttributes);
        return;
    }

    bool isFirstElement = !m_sawFirstElement;
    m_sawFirstElement = true;

    exitText();

    String localName = toString(xmlLocalName);
    String uri = toString(xmlURI);
    String prefix = toString(xmlPrefix);

    if (m_parsingFragment && uri.isNull()) {
        if (!prefix.isNull())
            uri = m_prefixToNamespaceMap.get(prefix);
        else
            uri = m_defaultNamespaceURI;
    }

    QualifiedName qName(prefix, localName, uri);
    RefPtr<Element> newElement = m_doc->createElement(qName, true);
    if (!newElement) {
        stopParsing();
        return;
    }
    
    ExceptionCode ec = 0;
    handleElementNamespaces(newElement.get(), libxmlNamespaces, nb_namespaces, ec);
    if (ec) {
        stopParsing();
        return;
    }

    ScriptController* jsProxy = m_doc->frame() ? m_doc->frame()->script() : 0;
    if (jsProxy && m_doc->frame()->script()->isEnabled())
        jsProxy->setEventHandlerLineNumber(lineNumber());

    handleElementAttributes(newElement.get(), libxmlAttributes, nb_attributes, ec);
    if (ec) {
        stopParsing();
        return;
    }

    if (jsProxy)
        jsProxy->setEventHandlerLineNumber(0);

    newElement->beginParsingChildren();

    ScriptElement* scriptElement = toScriptElement(newElement.get());
    if (scriptElement)
        m_scriptStartLine = lineNumber();

    if (!m_currentNode->addChild(newElement.get())) {
        stopParsing();
        return;
    }

    setCurrentNode(newElement.get());
    if (m_view && !newElement->attached())
        newElement->attach();

    if (isFirstElement && m_doc->frame())
        m_doc->frame()->loader()->dispatchDocumentElementAvailable();
}
