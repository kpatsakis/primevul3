HTMLSelectElement* HTMLKeygenElement::shadowSelect() const
{
    ShadowRoot* root = userAgentShadowRoot();
    return root ? toHTMLSelectElement(root->firstChild()) : 0;
}
