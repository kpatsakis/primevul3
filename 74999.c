void InlineFlowBox::checkConsistency() const
{
#ifdef CHECK_CONSISTENCY
    ASSERT(!m_hasBadChildList);
    const InlineBox* prev = 0;
    for (const InlineBox* child = m_firstChild; child; child = child->nextOnLine()) {
        ASSERT(child->parent() == this);
        ASSERT(child->prevOnLine() == prev);
        prev = child;
    }
    ASSERT(prev == m_lastChild);
#endif
}
