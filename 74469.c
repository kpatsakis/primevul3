void XMLTokenizer::internalSubset(const xmlChar* name, const xmlChar* externalID, const xmlChar* systemID)
{
    if (m_parserStopped)
        return;

    if (m_parserPaused) {
        m_pendingCallbacks->appendInternalSubsetCallback(name, externalID, systemID);
        return;
    }
    
    if (m_doc) {
#if ENABLE(WML)
        String extId = toString(externalID);
        if (isWMLDocument()
            && extId != "-//WAPFORUM//DTD WML 1.3//EN"
            && extId != "-//WAPFORUM//DTD WML 1.2//EN"
            && extId != "-//WAPFORUM//DTD WML 1.1//EN"
            && extId != "-//WAPFORUM//DTD WML 1.0//EN")
            handleError(fatal, "Invalid DTD Public ID", lineNumber(), columnNumber());
#endif

        m_doc->addChild(DocumentType::create(m_doc, toString(name), toString(externalID), toString(systemID)));
    }
}
