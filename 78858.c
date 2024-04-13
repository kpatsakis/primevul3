void Element::detachAttrNodeFromElementWithValue(Attr* attrNode, const AtomicString& value)
{
    ASSERT(hasSyntheticAttrChildNodes());
    attrNode->detachFromElementWithValue(value);

    AttrNodeList* attrNodeList = attrNodeListForElement(this);
    for (unsigned i = 0; i < attrNodeList->size(); ++i) {
        if (attrNodeList->at(i)->qualifiedName() == attrNode->qualifiedName()) {
            attrNodeList->remove(i);
            if (attrNodeList->isEmpty())
                removeAttrNodeListForElement(this);
            return;
        }
    }
    ASSERT_NOT_REACHED();
}
