static bool shouldSkipWhitespaceAfterStartObject(RenderBlock* block, RenderObject* o, LineMidpointState& lineMidpointState)
{
    RenderObject* next = bidiNextSkippingEmptyInlines(block, o);
    while (next && next->isFloatingOrOutOfFlowPositioned())
        next = bidiNextSkippingEmptyInlines(block, next);

    if (next && !next->isBR() && next->isText() && toRenderText(next)->textLength() > 0) {
        RenderText* nextText = toRenderText(next);
        UChar nextChar = nextText->characterAt(0);
        if (nextText->style()->isCollapsibleWhiteSpace(nextChar)) {
            startIgnoringSpaces(lineMidpointState, InlineIterator(0, o, 0));
            return true;
        }
    }

    return false;
}
