XSLStyleSheet::~XSLStyleSheet()
{
    if (!m_stylesheetDocTaken)
        xmlFreeDoc(m_stylesheetDoc);

    for (unsigned i = 0; i < m_children.size(); ++i) {
        ASSERT(m_children.at(i)->parentStyleSheet() == this);
        m_children.at(i)->setParentStyleSheet(0);
    }
}
