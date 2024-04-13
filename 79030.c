Element::~Element()
{
#ifndef NDEBUG
    if (document() && document()->renderer()) {
        ASSERT(!inNamedFlow());
    }
#endif

    if (hasRareData()) {
        ElementRareData* data = elementRareData();
        data->setPseudoElement(BEFORE, 0);
        data->setPseudoElement(AFTER, 0);
        data->clearShadow();
    }

    if (isCustomElement() && document() && document()->registry()) {
        document()->registry()->customElementWasDestroyed(this);
    }

    if (hasSyntheticAttrChildNodes())
        detachAllAttrNodesFromElement();

    if (hasPendingResources()) {
        document()->accessSVGExtensions()->removeElementFromPendingResources(this);
        ASSERT(!hasPendingResources());
    }
}
