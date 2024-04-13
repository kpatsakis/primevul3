bool HTMLSelectElement::usesMenuList() const
{
    if (RenderTheme::theme().delegatesMenuListRendering())
        return true;

    return !m_multiple && m_size <= 1;
}
