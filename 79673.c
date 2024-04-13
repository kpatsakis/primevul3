HTMLElement* HTMLInputElement::passwordGeneratorButtonElement() const
{
    return toHTMLElement(userAgentShadowRoot()->getElementById(ShadowElementNames::passwordGenerator()));
}
