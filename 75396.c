void RenderMenuList::listBoxSelectItem(int listIndex, bool allowMultiplySelections, bool shift, bool fireOnChangeNow)
{
     toHTMLSelectElement(node())->listBoxSelectItem(listIndex, allowMultiplySelections, shift, fireOnChangeNow);
 }
