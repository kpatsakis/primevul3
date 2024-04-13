void HTMLInputElement::setIndeterminate(bool newValue)
{
    if (indeterminate() == newValue)
        return;

    m_isIndeterminate = newValue;

    didAffectSelector(AffectedSelectorIndeterminate);

    if (renderer() && renderer()->style()->hasAppearance())
        RenderTheme::theme().stateChanged(renderer(), CheckedState);
}
