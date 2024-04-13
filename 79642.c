void HTMLFormControlElement::updateAncestorDisabledState() const
{
    HTMLFieldSetElement* fieldSetAncestor = 0;
    ContainerNode* legendAncestor = 0;
    for (ContainerNode* ancestor = parentNode(); ancestor; ancestor = ancestor->parentNode()) {
        if (!legendAncestor && ancestor->hasTagName(legendTag))
            legendAncestor = ancestor;
        if (ancestor->hasTagName(fieldsetTag)) {
            fieldSetAncestor = toHTMLFieldSetElement(ancestor);
            break;
        }
    }
    m_ancestorDisabledState = (fieldSetAncestor && fieldSetAncestor->isDisabledFormControl() && !(legendAncestor && legendAncestor == fieldSetAncestor->legend())) ? AncestorDisabledStateDisabled : AncestorDisabledStateEnabled;
}
