static PassRefPtr<Range> characterSubrange(CharacterIterator& it, int offset, int length)
{
    it.advance(offset);
    RefPtr<Range> start = it.range();

    if (length > 1)
        it.advance(length - 1);
    RefPtr<Range> end = it.range();

    return Range::create(start->startContainer()->document(), 
        start->startContainer(), start->startOffset(), 
        end->endContainer(), end->endOffset());
}
