void HTMLTextAreaElement::handleFocusEvent(Element*, FocusDirection)
{
    if (Frame* frame = document().frame())
        frame->spellChecker().didBeginEditing(this);
}
