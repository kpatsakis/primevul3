static size_t findPlainText(CharacterIterator& it, const String& target, FindOptions options, size_t& matchStart)
{
    matchStart = 0;
    size_t matchLength = 0;

    SearchBuffer buffer(target, options);

    if (buffer.needsMoreContext()) {
        RefPtr<Range> startRange = it.range();
        RefPtr<Range> beforeStartRange = startRange->ownerDocument()->createRange();
        beforeStartRange->setEnd(startRange->startContainer(), startRange->startOffset(), IGNORE_EXCEPTION);
        for (SimplifiedBackwardsTextIterator backwardsIterator(beforeStartRange.get()); !backwardsIterator.atEnd(); backwardsIterator.advance()) {
            buffer.prependContext(backwardsIterator.characters(), backwardsIterator.length());
            if (!buffer.needsMoreContext())
                break;
        }
    }

    while (!it.atEnd()) {
        it.advance(buffer.append(it.characters(), it.length()));
tryAgain:
        size_t matchStartOffset;
        if (size_t newMatchLength = buffer.search(matchStartOffset)) {
            size_t lastCharacterInBufferOffset = it.characterOffset();
            ASSERT(lastCharacterInBufferOffset >= matchStartOffset);
            matchStart = lastCharacterInBufferOffset - matchStartOffset;
            matchLength = newMatchLength;
            if (!(options & Backwards))
                break;
            goto tryAgain;
        }
        if (it.atBreak() && !buffer.atBreak()) {
            buffer.reachedBreak();
            goto tryAgain;
        }
    }

    return matchLength;
}
