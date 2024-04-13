PassRefPtr<Range> TextIterator::rangeFromLocationAndLength(ContainerNode* scope, int rangeLocation, int rangeLength, bool forSelectionPreservation)
{
    RefPtr<Range> resultRange = scope->document()->createRange();

    int docTextPosition = 0;
    int rangeEnd = rangeLocation + rangeLength;
    bool startRangeFound = false;

    RefPtr<Range> textRunRange;

    TextIterator it(rangeOfContents(scope).get(), forSelectionPreservation ? TextIteratorEmitsCharactersBetweenAllVisiblePositions : TextIteratorDefaultBehavior);
    
    if (rangeLocation == 0 && rangeLength == 0 && it.atEnd()) {
        textRunRange = it.range();
        
        resultRange->setStart(textRunRange->startContainer(), 0, ASSERT_NO_EXCEPTION);
        resultRange->setEnd(textRunRange->startContainer(), 0, ASSERT_NO_EXCEPTION);
        
        return resultRange.release();
    }

    for (; !it.atEnd(); it.advance()) {
        int len = it.length();
        textRunRange = it.range();
        
        bool foundStart = rangeLocation >= docTextPosition && rangeLocation <= docTextPosition + len;
        bool foundEnd = rangeEnd >= docTextPosition && rangeEnd <= docTextPosition + len;
        
        if (foundEnd) {
            if (len == 1 && it.characterAt(0) == '\n') {
                scope->document()->updateLayoutIgnorePendingStylesheets();
                it.advance();
                if (!it.atEnd()) {
                    RefPtr<Range> range = it.range();
                    textRunRange->setEnd(range->startContainer(), range->startOffset(), ASSERT_NO_EXCEPTION);
                } else {
                    Position runStart = textRunRange->startPosition();
                    Position runEnd = VisiblePosition(runStart).next().deepEquivalent();
                    if (runEnd.isNotNull())
                        textRunRange->setEnd(runEnd.containerNode(), runEnd.computeOffsetInContainerNode(), ASSERT_NO_EXCEPTION);
                }
            }
        }

        if (foundStart) {
            startRangeFound = true;
            int exception = 0;
            if (textRunRange->startContainer()->isTextNode()) {
                int offset = rangeLocation - docTextPosition;
                resultRange->setStart(textRunRange->startContainer(), offset + textRunRange->startOffset(), exception);
            } else {
                if (rangeLocation == docTextPosition)
                    resultRange->setStart(textRunRange->startContainer(), textRunRange->startOffset(), exception);
                else
                    resultRange->setStart(textRunRange->endContainer(), textRunRange->endOffset(), exception);
            }
        }

        if (foundEnd) {
            int exception = 0;
            if (textRunRange->startContainer()->isTextNode()) {
                int offset = rangeEnd - docTextPosition;
                resultRange->setEnd(textRunRange->startContainer(), offset + textRunRange->startOffset(), exception);
            } else {
                if (rangeEnd == docTextPosition)
                    resultRange->setEnd(textRunRange->startContainer(), textRunRange->startOffset(), exception);
                else
                    resultRange->setEnd(textRunRange->endContainer(), textRunRange->endOffset(), exception);
            }
            docTextPosition += len;
            break;
        }
        docTextPosition += len;
    }
    
    if (!startRangeFound)
        return 0;
    
    if (rangeLength != 0 && rangeEnd > docTextPosition) { // rangeEnd is out of bounds
        int exception = 0;
        resultRange->setEnd(textRunRange->endContainer(), textRunRange->endOffset(), exception);
    }
    
    return resultRange.release();
}
