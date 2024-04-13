void HTMLTextAreaElement::didAddUserAgentShadowRoot(ShadowRoot& root)
{
    root.appendChild(TextControlInnerTextElement::create(document()));
}
