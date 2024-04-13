ScriptLoader::ScriptLoader(Element* element, bool parserInserted, bool alreadyStarted)
    : m_element(element)
    , m_resource(0)
    , m_startLineNumber(WTF::OrdinalNumber::beforeFirst())
    , m_parserInserted(parserInserted)
    , m_isExternalScript(false)
    , m_alreadyStarted(alreadyStarted)
    , m_haveFiredLoad(false)
    , m_willBeParserExecuted(false)
    , m_readyToBeParserExecuted(false)
    , m_willExecuteWhenDocumentFinishedParsing(false)
    , m_forceAsync(!parserInserted)
    , m_willExecuteInOrder(false)
{
    ASSERT(m_element);
    if (parserInserted && element->document().scriptableDocumentParser() && !element->document().isInDocumentWrite())
        m_startLineNumber = element->document().scriptableDocumentParser()->lineNumber();
}
