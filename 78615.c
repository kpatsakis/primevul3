xmlDocPtr XSLStyleSheet::document()
{
    if (m_embedded && ownerDocument() && ownerDocument()->transformSource())
        return (xmlDocPtr)ownerDocument()->transformSource()->platformSource();
    return m_stylesheetDoc;
}
