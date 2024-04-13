PassRefPtr<Element> Element::cloneElementWithoutAttributesAndChildren()
{
    return document()->createElement(tagQName(), false);
}
