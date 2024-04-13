void XSLStyleSheet::loadChildSheets()
{
    if (!document())
        return;

    xmlNodePtr stylesheetRoot = document()->children;

    while (stylesheetRoot && stylesheetRoot->type != XML_ELEMENT_NODE)
        stylesheetRoot = stylesheetRoot->next;

    if (m_embedded) {
        xmlAttrPtr idNode = xmlGetID(document(), (const xmlChar*)(finalURL().string().utf8().data()));
        if (!idNode)
            return;
        stylesheetRoot = idNode->parent;
    } else {
    }

    if (stylesheetRoot) {
        xmlNodePtr curr = stylesheetRoot->children;
        while (curr) {
            if (curr->type != XML_ELEMENT_NODE) {
                curr = curr->next;
                continue;
            }
            if (IS_XSLT_ELEM(curr) && IS_XSLT_NAME(curr, "import")) {
                xmlChar* uriRef = xsltGetNsProp(curr, (const xmlChar*)"href", XSLT_NAMESPACE);
                loadChildSheet(String::fromUTF8((const char*)uriRef));
                xmlFree(uriRef);
            } else
                break;
            curr = curr->next;
        }

        while (curr) {
            if (curr->type == XML_ELEMENT_NODE && IS_XSLT_ELEM(curr) && IS_XSLT_NAME(curr, "include")) {
                xmlChar* uriRef = xsltGetNsProp(curr, (const xmlChar*)"href", XSLT_NAMESPACE);
                loadChildSheet(String::fromUTF8((const char*)uriRef));
                xmlFree(uriRef);
            }
            curr = curr->next;
        }
    }
}
