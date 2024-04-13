void HTMLInputElement::didMoveToNewDocument(Document& oldDocument)
{
    if (hasImageLoader())
        imageLoader()->elementDidMoveToNewDocument();

    if (isRadioButton())
        oldDocument.formController()->checkedRadioButtons().removeButton(this);
    if (m_hasTouchEventHandler)
        oldDocument.didRemoveEventTargetNode(this);

    if (m_hasTouchEventHandler)
        document().didAddTouchEventHandler(this);

    HTMLTextFormControlElement::didMoveToNewDocument(oldDocument);
}
