void HTMLTextAreaElement::childrenChanged(bool changedByParser, Node* beforeChange, Node* afterChange, int childCountDelta)
{
    HTMLElement::childrenChanged(changedByParser, beforeChange, afterChange, childCountDelta);
    setLastChangeWasNotUserEdit();
    if (m_isDirty)
        setInnerTextValue(value());
    else
        setNonDirtyValue(defaultValue());
}
