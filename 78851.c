PassRefPtr<ShadowRoot> Element::createShadowRoot(ExceptionCode& ec)
{
    if (alwaysCreateUserAgentShadowRoot())
        ensureUserAgentShadowRoot();

    if (RuntimeEnabledFeatures::authorShadowDOMForAnyElementEnabled())
        return ensureShadow()->addShadowRoot(this, ShadowRoot::AuthorShadowRoot);

    if (!areAuthorShadowsAllowed()) {
        ec = HIERARCHY_REQUEST_ERR;
        return 0;
    }
    return ensureShadow()->addShadowRoot(this, ShadowRoot::AuthorShadowRoot);
}
