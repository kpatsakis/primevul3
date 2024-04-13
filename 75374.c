void RenderMenuList::addChild(RenderObject* newChild, RenderObject* beforeChild)
{
    createInnerBlock();
    m_innerBlock->addChild(newChild, beforeChild);
    ASSERT(m_innerBlock == firstChild());
}
