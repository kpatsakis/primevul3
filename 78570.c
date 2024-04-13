static bool requiresLineBoxForContent(RenderInline* flow, const LineInfo& lineInfo)
{
    RenderObject* parent = flow->parent();
    if (flow->document().inNoQuirksMode()
        && (flow->style(lineInfo.isFirstLine())->lineHeight() != parent->style(lineInfo.isFirstLine())->lineHeight()
        || flow->style()->verticalAlign() != parent->style()->verticalAlign()
        || !parent->style()->font().fontMetrics().hasIdenticalAscentDescentAndLineGap(flow->style()->font().fontMetrics())))
        return true;
    return false;
}
