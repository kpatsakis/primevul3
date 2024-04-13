void DocumentWriter::setDocumentWasLoadedAsPartOfNavigation()
{
    ASSERT(!m_parser->isStopped());
    m_parser->setDocumentWasLoadedAsPartOfNavigation();
}
