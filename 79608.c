void HTMLFormControlElement::didMoveToNewDocument(Document& oldDocument)
{
    FormAssociatedElement::didMoveToNewDocument(oldDocument);
    HTMLElement::didMoveToNewDocument(oldDocument);
}
