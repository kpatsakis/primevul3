PassRefPtr<Range> TextIterator::subrange(Range* entireRange, int characterOffset, int characterCount)
{
    CharacterIterator entireRangeIterator(entireRange);
    return characterSubrange(entireRangeIterator, characterOffset, characterCount);
}
