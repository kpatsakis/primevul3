void Element::attach(const AttachContext& context)
{
    PostAttachCallbackDisabler callbackDisabler(this);
    StyleResolverParentPusher parentPusher(this);
    WidgetHierarchyUpdatesSuspensionScope suspendWidgetHierarchyUpdates;

    createRendererIfNeeded(context);

    if (parentElement() && parentElement()->isInCanvasSubtree())
        setIsInCanvasSubtree(true);

    createPseudoElementIfNeeded(BEFORE);

    if (ElementShadow* shadow = this->shadow()) {
        parentPusher.push();
        shadow->attach(context);
    } else if (firstChild())
        parentPusher.push();

    ContainerNode::attach(context);

    createPseudoElementIfNeeded(AFTER);

    if (hasRareData()) {   
        ElementRareData* data = elementRareData();
        if (data->needsFocusAppearanceUpdateSoonAfterAttach()) {
            if (isFocusable() && document()->focusedNode() == this)
                document()->updateFocusAppearanceSoon(false /* don't restore selection */);
            data->setNeedsFocusAppearanceUpdateSoonAfterAttach(false);
        }
    }

    if (document()->inStyleRecalc())
        InspectorInstrumentation::didRecalculateStyleForElement(this);
}
