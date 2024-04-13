void HTMLFormElement::removedFrom(ContainerNode* insertionPoint)
{
    Node* root = findRoot(this);
    Vector<FormAssociatedElement*> associatedElements(m_associatedElements);
    for (unsigned i = 0; i < associatedElements.size(); ++i)
        associatedElements[i]->formRemovedFromTree(root);
    HTMLElement::removedFrom(insertionPoint);
}
