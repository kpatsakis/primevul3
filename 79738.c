void HTMLSelectElement::resetImpl()
{
    HTMLOptionElement* firstOption = 0;
    HTMLOptionElement* selectedOption = 0;

    const Vector<HTMLElement*>& items = listItems();
    for (unsigned i = 0; i < items.size(); ++i) {
        HTMLElement* element = items[i];
        if (!element->hasTagName(optionTag))
            continue;

        if (items[i]->fastHasAttribute(selectedAttr)) {
            if (selectedOption && !m_multiple)
                selectedOption->setSelectedState(false);
            toHTMLOptionElement(element)->setSelectedState(true);
            selectedOption = toHTMLOptionElement(element);
        } else
            toHTMLOptionElement(element)->setSelectedState(false);

        if (!firstOption)
            firstOption = toHTMLOptionElement(element);
    }

    if (!selectedOption && firstOption && !m_multiple && m_size <= 1)
        firstOption->setSelectedState(true);

    setOptionsChangedOnRenderer();
    setNeedsStyleRecalc();
    setNeedsValidityCheck();
}
