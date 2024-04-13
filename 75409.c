void RenderMenuList::valueChanged(unsigned listIndex, bool fireOnChange)
{
    Document* doc = static_cast<Element*>(node())->document();
    if (!doc || doc != doc->frame()->document())
        return;
    
    HTMLSelectElement* select = toHTMLSelectElement(node());
    select->optionSelectedByUser(select->listToOptionIndex(listIndex), fireOnChange);
}
