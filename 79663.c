void HTMLInputElement::didAddUserAgentShadowRoot(ShadowRoot&)
{
    m_inputTypeView->createShadowSubtree();
}
