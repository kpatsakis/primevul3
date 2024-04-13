static inline bool iteratorIsBeyondEndOfRenderCombineText(const InlineIterator& iter, RenderCombineText* renderer)
{
    return iter.m_obj == renderer && iter.m_pos >= renderer->textLength();
}
