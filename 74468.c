void XMLTokenizer::initializeParserContext(const char* chunk)
{
    xmlSAXHandler sax;
    memset(&sax, 0, sizeof(sax));

    sax.error = normalErrorHandler;
    sax.fatalError = fatalErrorHandler;
    sax.characters = charactersHandler;
    sax.processingInstruction = processingInstructionHandler;
    sax.cdataBlock = cdataBlockHandler;
    sax.comment = commentHandler;
    sax.warning = warningHandler;
    sax.startElementNs = startElementNsHandler;
    sax.endElementNs = endElementNsHandler;
    sax.getEntity = getEntityHandler;
    sax.startDocument = startDocumentHandler;
    sax.endDocument = endDocumentHandler;
    sax.internalSubset = internalSubsetHandler;
    sax.externalSubset = externalSubsetHandler;
    sax.ignorableWhitespace = ignorableWhitespaceHandler;
    sax.entityDecl = xmlSAX2EntityDecl;
    sax.initialized = XML_SAX2_MAGIC;
    m_parserStopped = false;
    m_sawError = false;
    m_sawXSLTransform = false;
    m_sawFirstElement = false;

    XMLTokenizerScope scope(m_doc->docLoader());
    if (m_parsingFragment)
        m_context = createMemoryParser(&sax, this, chunk);
    else
        m_context = createStringParser(&sax, this);
}
