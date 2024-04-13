AtomicString Element::computeInheritedLanguage() const
{
    const Node* n = this;
    AtomicString value;
    do {
        if (n->isElementNode()) {
            if (const ElementData* elementData = toElement(n)->elementData()) {
                if (const Attribute* attribute = elementData->getAttributeItem(XMLNames::langAttr))
                    value = attribute->value();
                else if (const Attribute* attribute = elementData->getAttributeItem(HTMLNames::langAttr))
                    value = attribute->value();
            }
        } else if (n->isDocumentNode()) {
            value = toDocument(n)->contentLanguage();
        }

        n = n->parentNode();
    } while (n && value.isNull());

    return value;
}
