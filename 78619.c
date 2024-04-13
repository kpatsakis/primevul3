xmlDocPtr XSLStyleSheet::locateStylesheetSubResource(xmlDocPtr parentDoc, const xmlChar* uri)
{
    bool matchedParent = (parentDoc == document());
    for (unsigned i = 0; i < m_children.size(); ++i) {
        XSLImportRule* import = m_children.at(i).get();
        XSLStyleSheet* child = import->styleSheet();
        if (!child)
            continue;
        if (matchedParent) {
            if (child->processed())
                continue; // libxslt has been given this sheet already.

            CString importHref = import->href().utf8();
            xmlChar* base = xmlNodeGetBase(parentDoc, (xmlNodePtr)parentDoc);
            xmlChar* childURI = xmlBuildURI((const xmlChar*)importHref.data(), base);
            bool equalURIs = xmlStrEqual(uri, childURI);
            xmlFree(base);
            xmlFree(childURI);
            if (equalURIs) {
                child->markAsProcessed();
                return child->document();
            }
            continue;
        }
        xmlDocPtr result = import->styleSheet()->locateStylesheetSubResource(parentDoc, uri);
        if (result)
            return result;
    }

    return 0;
}
