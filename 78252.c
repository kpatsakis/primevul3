void HTMLFormElement::registerFormElement(FormAssociatedElement* e)
{
    m_associatedElements.insert(formElementIndex(e), e);
}
