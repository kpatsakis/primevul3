PassRefPtr<HTMLTextAreaElement> HTMLTextAreaElement::create(Document& document, HTMLFormElement* form)
{
    RefPtr<HTMLTextAreaElement> textArea = adoptRef(new HTMLTextAreaElement(document, form));
    textArea->ensureUserAgentShadowRoot();
    return textArea.release();
}
