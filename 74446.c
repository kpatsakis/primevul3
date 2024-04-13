int XMLTokenizer::columnNumber() const
{
    return m_context ? m_context->input->col : 1;
}
