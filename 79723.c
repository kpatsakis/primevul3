PassRefPtr<HTMLSelectElement> HTMLSelectElement::create(Document& document)
{
    return adoptRef(new HTMLSelectElement(document, 0, false));
}
