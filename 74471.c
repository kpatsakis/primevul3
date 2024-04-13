int XMLTokenizer::lineNumber() const
{
    return m_context ? m_context->input->line : 1;
}
