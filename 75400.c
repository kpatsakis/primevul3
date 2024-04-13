void RenderMenuList::removeChild(RenderObject* oldChild)
{
    if (oldChild == m_innerBlock || !m_innerBlock) {
        RenderDeprecatedFlexibleBox::removeChild(oldChild);
        m_innerBlock = 0;
    } else
        m_innerBlock->removeChild(oldChild);
}
