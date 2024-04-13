void RenderMenuList::createInnerBlock()
{
    if (m_innerBlock) {
        ASSERT(firstChild() == m_innerBlock);
        ASSERT(!m_innerBlock->nextSibling());
        return;
    }

    ASSERT(!firstChild());
    m_innerBlock = createAnonymousBlock();
    adjustInnerStyle();
    RenderDeprecatedFlexibleBox::addChild(m_innerBlock);
}
