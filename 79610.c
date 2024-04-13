void HTMLFormControlElement::disabledAttributeChanged()
{
    setNeedsWillValidateCheck();
    didAffectSelector(AffectedSelectorDisabled | AffectedSelectorEnabled);
    if (renderer() && renderer()->style()->hasAppearance())
        RenderTheme::theme().stateChanged(renderer(), EnabledState);
    if (isDisabledFormControl() && treeScope().adjustedFocusedElement() == this) {
        document().setNeedsFocusedElementCheck();
    }
}
