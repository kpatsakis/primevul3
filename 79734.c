void HTMLSelectElement::optionSelectionStateChanged(HTMLOptionElement* option, bool optionIsSelected)
{
    ASSERT(option->ownerSelectElement() == this);
    if (optionIsSelected)
        selectOption(option->index());
    else if (!usesMenuList() || multiple())
        selectOption(-1);
    else
        selectOption(nextSelectableListIndex(-1));
}
