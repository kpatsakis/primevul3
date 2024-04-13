PassRefPtr<HTMLInputElement> HTMLInputElement::create(Document& document, HTMLFormElement* form, bool createdByParser)
{
    RefPtr<HTMLInputElement> inputElement = adoptRef(new HTMLInputElement(document, form, createdByParser));
    inputElement->ensureUserAgentShadowRoot();
    return inputElement.release();
}
