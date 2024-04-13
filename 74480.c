void XMLTokenizer::startDocument(const xmlChar* version, const xmlChar* encoding, int standalone)
{
    ExceptionCode ec = 0;

    if (version)
        m_doc->setXMLVersion(toString(version), ec);
    m_doc->setXMLStandalone(standalone == 1, ec); // possible values are 0, 1, and -1
    if (encoding)
        m_doc->setXMLEncoding(toString(encoding));
}
