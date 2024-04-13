void XMLTokenizer::processingInstruction(const xmlChar* target, const xmlChar* data)
{
    if (m_parserStopped)
        return;

    if (m_parserPaused) {
        m_pendingCallbacks->appendProcessingInstructionCallback(target, data);
        return;
    }
    
    exitText();

    int exception = 0;
    RefPtr<ProcessingInstruction> pi = m_doc->createProcessingInstruction(
        toString(target), toString(data), exception);
    if (exception)
        return;

    pi->setCreatedByParser(true);
    
    if (!m_currentNode->addChild(pi.get()))
        return;
    if (m_view && !pi->attached())
        pi->attach();

    pi->finishParsingChildren();

#if ENABLE(XSLT)
    m_sawXSLTransform = !m_sawFirstElement && pi->isXSL();
    if (m_sawXSLTransform && !m_doc->transformSourceDocument())
        stopParsing();
#endif
}
