static bool requiresLineBox(const InlineIterator& it, const LineInfo& lineInfo = LineInfo(), WhitespacePosition whitespacePosition = LeadingWhitespace)
{
    if (it.m_obj->isFloatingOrOutOfFlowPositioned())
        return false;

    if (it.m_obj->isRenderInline() && !alwaysRequiresLineBox(it.m_obj) && !requiresLineBoxForContent(toRenderInline(it.m_obj), lineInfo))
        return false;

    if (!shouldCollapseWhiteSpace(it.m_obj->style(), lineInfo, whitespacePosition) || it.m_obj->isBR())
        return true;

    UChar current = it.current();
    bool notJustWhitespace = current != ' ' && current != '\t' && current != softHyphen && (current != '\n' || it.m_obj->preservesNewline());
    return notJustWhitespace || isEmptyInline(it.m_obj);
}
