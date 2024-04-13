bool TextIterator::getLocationAndLengthFromRange(Node* scope, const Range* range, size_t& location, size_t& length)
{
    location = notFound;
    length = 0;

    if (!range->startContainer())
        return false;

    if (range->startContainer() != scope && !range->startContainer()->isDescendantOf(scope))
        return false;
    if (range->endContainer() != scope && !range->endContainer()->isDescendantOf(scope))
        return false;

    RefPtr<Range> testRange = Range::create(scope->document(), scope, 0, range->startContainer(), range->startOffset());
    ASSERT(testRange->startContainer() == scope);
    location = TextIterator::rangeLength(testRange.get());

    testRange->setEnd(range->endContainer(), range->endOffset(), IGNORE_EXCEPTION);
    ASSERT(testRange->startContainer() == scope);
    length = TextIterator::rangeLength(testRange.get()) - location;
    return true;
}
