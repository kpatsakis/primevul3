void XMLTokenizer::endElementNs()
{
    if (m_parserStopped)
        return;

    if (m_parserPaused) {
        m_pendingCallbacks->appendEndElementNSCallback();
        return;
    }
    
    exitText();

    Node* n = m_currentNode;
    RefPtr<Node> parent = n->parentNode();
    n->finishParsingChildren();

    if (!n->isElementNode() || !m_view) {
        setCurrentNode(parent.get());
        return;
    }

    Element* element = static_cast<Element*>(n);
    ScriptElement* scriptElement = toScriptElement(element);
    if (!scriptElement) {
        setCurrentNode(parent.get());
        return;
    }

    ASSERT(!m_pendingScript);
    m_requestingScript = true;

    String scriptHref = scriptElement->sourceAttributeValue();
    if (!scriptHref.isEmpty()) {
        String scriptCharset = scriptElement->scriptCharset();
        if ((m_pendingScript = m_doc->docLoader()->requestScript(scriptHref, scriptCharset))) {
            m_scriptElement = element;
            m_pendingScript->addClient(this);

            if (m_pendingScript)
                pauseParsing();
        } else 
            m_scriptElement = 0;
    } else
        m_view->frame()->loader()->executeScript(ScriptSourceCode(scriptElement->scriptContent(), m_doc->url(), m_scriptStartLine));

    m_requestingScript = false;
    setCurrentNode(parent.get());
}
