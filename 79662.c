void HTMLInputElement::didAddShadowRoot(ShadowRoot& root)
{
    if (!root.isOldestAuthorShadowRoot())
        return;
    m_inputTypeView->destroyShadowSubtree();
    m_inputTypeView = InputTypeView::create(*this);
    lazyReattachIfAttached();
}
