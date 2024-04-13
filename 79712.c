HTMLKeygenElement::HTMLKeygenElement(Document& document, HTMLFormElement* form)
    : HTMLFormControlElementWithState(keygenTag, document, form)
{
    ScriptWrappable::init(this);
    ensureUserAgentShadowRoot();
}
