void XSLStyleSheet::markAsProcessed()
{
    ASSERT(!m_processed);
    ASSERT(!m_stylesheetDocTaken);
    m_processed = true;
    m_stylesheetDocTaken = true;
}
