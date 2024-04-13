int TextIterator::rangeLength(const Range* r, bool forSelectionPreservation)
{
    int length = 0;
    for (TextIterator it(r, forSelectionPreservation ? TextIteratorEmitsCharactersBetweenAllVisiblePositions : TextIteratorDefaultBehavior); !it.atEnd(); it.advance())
        length += it.length();
    
    return length;
}
