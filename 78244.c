Node::InsertionNotificationRequest HTMLFormElement::insertedInto(ContainerNode* insertionPoint)
{
    HTMLElement::insertedInto(insertionPoint);
    if (insertionPoint->inDocument())
        this->document().didAssociateFormControl(this);
    return InsertionDone;
}
