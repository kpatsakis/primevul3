bool CSSComputedStyleDeclaration::useFixedFontDefaultSize() const
{
    if (!m_node)
        return false;

    RefPtr<RenderStyle> style = m_node->computedStyle(m_pseudoElementSpecifier);
    if (!style)
        return false;

    return style->fontDescription().useFixedDefaultSize();
}
