static void checkForEmptyStyleChange(Element* element, RenderStyle* style)
{
    if (!style && !element->styleAffectedByEmpty())
        return;

    if (!style || (element->styleAffectedByEmpty() && (!style->emptyState() || element->hasChildNodes())))
        element->setNeedsStyleRecalc();
}
