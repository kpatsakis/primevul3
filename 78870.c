ShadowRoot* Element::ensureUserAgentShadowRoot()
{
    if (ShadowRoot* shadowRoot = userAgentShadowRoot())
        return shadowRoot;
    ShadowRoot* shadowRoot = ensureShadow()->addShadowRoot(this, ShadowRoot::UserAgentShadowRoot);
    didAddUserAgentShadowRoot(shadowRoot);
    return shadowRoot;
}
