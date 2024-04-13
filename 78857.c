void Element::detach(const AttachContext& context)
{
    WidgetHierarchyUpdatesSuspensionScope suspendWidgetHierarchyUpdates;
    unregisterNamedFlowContentNode();
    cancelFocusAppearanceUpdate();
    if (hasRareData()) {
        ElementRareData* data = elementRareData();
        data->setPseudoElement(BEFORE, 0);
        data->setPseudoElement(AFTER, 0);
        data->setIsInCanvasSubtree(false);
        data->resetComputedStyle();
        data->resetDynamicRestyleObservations();
    }
    if (ElementShadow* shadow = this->shadow())
        shadow->detach(context);
    ContainerNode::detach(context);
}
