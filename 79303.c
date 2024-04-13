PassRefPtr<Range> findPlainText(const Range* range, const String& target, FindOptions options)
{
    range->ownerDocument()->updateLayout();

    size_t matchStart;
    size_t matchLength;
    {
        CharacterIterator findIterator(range, TextIteratorEntersTextControls);
        matchLength = findPlainText(findIterator, target, options, matchStart);
        if (!matchLength)
            return collapsedToBoundary(range, !(options & Backwards));
    }

    CharacterIterator computeRangeIterator(range, TextIteratorEntersTextControls);
    return characterSubrange(computeRangeIterator, matchStart, matchLength);
}
