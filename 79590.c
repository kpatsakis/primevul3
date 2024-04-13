PassRefPtr<HTMLButtonElement> HTMLButtonElement::create(Document& document, HTMLFormElement* form)
{
    return adoptRef(new HTMLButtonElement(document, form));
}
