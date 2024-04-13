void HTMLSelectElement::setRecalcListItems()
{

    m_shouldRecalcListItems = true;
    m_activeSelectionAnchorIndex = -1;
    setOptionsChangedOnRenderer();
    setNeedsStyleRecalc();
    if (!inDocument()) {
        if (HTMLCollection* collection = cachedHTMLCollection(SelectOptions))
            collection->invalidateCache();
    }
    if (!inDocument())
        invalidateSelectedItems();

    if (renderer()) {
        if (AXObjectCache* cache = renderer()->document().existingAXObjectCache())
            cache->childrenChanged(this);
    }
}
