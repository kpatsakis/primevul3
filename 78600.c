ETextAlign RenderBlock::textAlignmentForLine(bool endsWithSoftBreak) const
{
    ETextAlign alignment = style()->textAlign();
    if (!endsWithSoftBreak && alignment == JUSTIFY)
        alignment = TASTART;

    return alignment;
}
