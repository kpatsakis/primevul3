void HTMLInputElement::setChecked(bool nowChecked, TextFieldEventBehavior eventBehavior)
{
    if (checked() == nowChecked)
        return;

    m_reflectsCheckedAttribute = false;
    m_isChecked = nowChecked;
    setNeedsStyleRecalc();

    if (CheckedRadioButtons* buttons = checkedRadioButtons())
            buttons->updateCheckedState(this);
    if (renderer() && renderer()->style()->hasAppearance())
        RenderTheme::theme().stateChanged(renderer(), CheckedState);

    setNeedsValidityCheck();

    if (renderer()) {
        if (AXObjectCache* cache = renderer()->document().existingAXObjectCache())
            cache->checkedStateChanged(this);
    }

    if (eventBehavior != DispatchNoEvent && inDocument() && m_inputType->shouldSendChangeEventAfterCheckedChanged()) {
        setTextAsOfLastFormControlChangeEvent(String());
        dispatchFormControlChangeEvent();
    }

    didAffectSelector(AffectedSelectorChecked);
}
