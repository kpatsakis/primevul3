PassRefPtr<Element> Element::create(const QualifiedName& tagName, Document* document)
{
    return adoptRef(new Element(tagName, document, CreateElement));
}
