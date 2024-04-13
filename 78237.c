void HTMLFormElement::finishParsingChildren()
{
    HTMLElement::finishParsingChildren();
    document().formController()->restoreControlStateIn(*this);
}
