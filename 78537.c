bool RenderBlock::generatesLineBoxesForInlineChild(RenderObject* inlineObj)
{
    ASSERT(inlineObj->parent() == this);

    InlineIterator it(this, inlineObj, 0);
    while (!it.atEnd() && !requiresLineBox(it))
        it.increment();

    return !it.atEnd();
}
