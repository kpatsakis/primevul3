bool TextIterator::hasVisibleTextNode(RenderText* renderer)
{
    if (renderer->style()->visibility() == VISIBLE)
        return true;
    if (renderer->isTextFragment()) {
        RenderTextFragment* fragment = static_cast<RenderTextFragment*>(renderer);
        if (fragment->firstLetter() && fragment->firstLetter()->style()->visibility() == VISIBLE)
            return true;
    }
    return false;
}
