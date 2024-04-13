static TextDirection determinePlaintextDirectionality(RenderObject* root, RenderObject* current = 0, unsigned pos = 0)
{
    InlineIterator iter(root, firstRenderObjectForDirectionalityDetermination(root, current), pos);
    InlineBidiResolver observer;
    observer.setPositionIgnoringNestedIsolates(iter);
    observer.setStatus(BidiStatus(root->style()->direction(), isOverride(root->style()->unicodeBidi())));
    while (!iter.atEnd()) {
        if (observer.inIsolate()) {
            iter.increment(&observer, InlineIterator::FastIncrementInlineRenderer);
            continue;
        }
        if (iter.atParagraphSeparator())
            break;
        if (UChar current = iter.current()) {
            Direction charDirection = direction(current);
            if (charDirection == LeftToRight)
                return LTR;
            if (charDirection == RightToLeft || charDirection == RightToLeftArabic)
                return RTL;
        }
        iter.increment(&observer);
    }
    return LTR;
}
