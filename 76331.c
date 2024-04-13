int InlineTextBox::lineHeight() const
{
    if (!isText() || !parent())
        return 0;
    if (m_renderer->isBR())
        return toRenderBR(m_renderer)->lineHeight(m_firstLine);
    return parent()->lineHeight();
}
