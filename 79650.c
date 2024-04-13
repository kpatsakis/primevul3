void HTMLInputElement::beginEditing()
{
    ASSERT(document().isActive());
    if (!document().isActive())
        return;

    if (!isTextField())
        return;

    document().frame()->spellChecker().didBeginEditing(this);
}
