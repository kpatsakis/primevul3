PassRefPtr<HTMLSelectElement> HTMLSelectElement::create(Document& document, HTMLFormElement* form, bool createdByParser)
{
    return adoptRef(new HTMLSelectElement(document, form, createdByParser));
}
