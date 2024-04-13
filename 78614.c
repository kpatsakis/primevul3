void XSLStyleSheet::clearDocuments()
{
    m_stylesheetDoc = 0;
    for (unsigned i = 0; i < m_children.size(); ++i) {
        XSLImportRule* import = m_children.at(i).get();
        if (import->styleSheet())
            import->styleSheet()->clearDocuments();
    }
}
