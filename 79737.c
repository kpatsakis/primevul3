void HTMLSelectElement::recalcListItems(bool updateSelectedStates) const
{
    m_listItems.clear();

    m_shouldRecalcListItems = false;

    HTMLOptionElement* foundSelected = 0;
    HTMLOptionElement* firstOption = 0;
    for (Element* currentElement = ElementTraversal::firstWithin(*this); currentElement; ) {
        if (!currentElement->isHTMLElement()) {
            currentElement = ElementTraversal::nextSkippingChildren(*currentElement, this);
            continue;
        }
        HTMLElement& current = toHTMLElement(*currentElement);

        if (current.hasTagName(optgroupTag)) {
            m_listItems.append(&current);
            if (Element* nextElement = ElementTraversal::firstWithin(current)) {
                currentElement = nextElement;
                continue;
            }
        }

        if (current.hasTagName(optionTag)) {
            m_listItems.append(&current);

            if (updateSelectedStates && !m_multiple) {
                HTMLOptionElement& option = toHTMLOptionElement(current);
                if (!firstOption)
                    firstOption = &option;
                if (option.selected()) {
                    if (foundSelected)
                        foundSelected->setSelectedState(false);
                    foundSelected = &option;
                } else if (m_size <= 1 && !foundSelected && !option.isDisabledFormControl()) {
                    foundSelected = &option;
                    foundSelected->setSelectedState(true);
                }
            }
        }

        if (current.hasTagName(hrTag))
            m_listItems.append(&current);

        currentElement = ElementTraversal::nextSkippingChildren(*currentElement, this);
    }

    if (!foundSelected && m_size <= 1 && firstOption && !firstOption->selected())
        firstOption->setSelectedState(true);
}
