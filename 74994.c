void InlineFlowBox::addToLine(InlineBox* child) 
{
    ASSERT(!child->parent());
    ASSERT(!child->nextOnLine());
    ASSERT(!child->prevOnLine());
    checkConsistency();

    child->setParent(this);
    if (!m_firstChild) {
        m_firstChild = child;
        m_lastChild = child;
    } else {
        m_lastChild->setNextOnLine(child);
        child->setPrevOnLine(m_lastChild);
        m_lastChild = child;
    }
    child->setFirstLineStyleBit(m_firstLine);
    child->setIsHorizontal(isHorizontal());
    if (child->isText())
        m_hasTextChildren = true;

    checkConsistency();
}
