void Element::recalcStyle(StyleChange change)
{
    ASSERT(document()->inStyleRecalc());

    if (hasCustomStyleCallbacks())
        willRecalcStyle(change);

    RefPtr<RenderStyle> currentStyle(renderStyle());
    bool hasParentStyle = parentNodeForRenderingAndStyle() ? static_cast<bool>(parentNodeForRenderingAndStyle()->renderStyle()) : false;
    bool hasDirectAdjacentRules = childrenAffectedByDirectAdjacentRules();
    bool hasIndirectAdjacentRules = childrenAffectedByForwardPositionalRules();

    if ((change > NoChange || needsStyleRecalc())) {
        if (hasRareData())
            elementRareData()->resetComputedStyle();
    }
    if (hasParentStyle && (change >= Inherit || needsStyleRecalc())) {
        StyleChange localChange = Detach;
        RefPtr<RenderStyle> newStyle;
        if (currentStyle) {
            newStyle = styleForRenderer();
            localChange = Node::diff(currentStyle.get(), newStyle.get(), document());
        } else if (attached() && isActiveInsertionPoint(this)) {
            localChange = change;
        }
        if (localChange == Detach) {
            AttachContext reattachContext;
            reattachContext.resolvedStyle = newStyle.get();
            reattach(reattachContext);

            clearNeedsStyleRecalc();
            clearChildNeedsStyleRecalc();

            if (hasCustomStyleCallbacks())
                didRecalcStyle(change);
            return;
        }

        InspectorInstrumentation::didRecalculateStyleForElement(this);

        if (RenderObject* renderer = this->renderer()) {
            if (localChange != NoChange || pseudoStyleCacheIsInvalid(currentStyle.get(), newStyle.get()) || (change == Force && renderer->requiresForcedStyleRecalcPropagation()) || styleChangeType() == SyntheticStyleChange)
                renderer->setAnimatableStyle(newStyle.get());
            else if (needsStyleRecalc()) {
                renderer->setStyleInternal(newStyle.get());
            }
        }

        if (document()->styleSheetCollection()->usesRemUnits() && document()->documentElement() == this && localChange != NoChange && currentStyle && newStyle && currentStyle->fontSize() != newStyle->fontSize()) {
            document()->styleResolver()->invalidateMatchedPropertiesCache();
            change = Force;
        }

        if (styleChangeType() == FullStyleChange)
            change = Force;
        else if (change != Force)
            change = localChange;
    }
    StyleResolverParentPusher parentPusher(this);

    if (ElementShadow* shadow = this->shadow()) {
        if (shouldRecalcStyle(change, shadow)) {
            parentPusher.push();
            shadow->recalcStyle(change);
        }
    }

    if (shouldRecalcStyle(change, this))
        updatePseudoElement(BEFORE, change);

    bool forceCheckOfNextElementSibling = false;
    bool forceCheckOfAnyElementSibling = false;
    for (Node *n = firstChild(); n; n = n->nextSibling()) {
        if (n->isTextNode()) {
            toText(n)->recalcTextStyle(change);
            continue;
        } 
        if (!n->isElementNode()) 
            continue;
        Element* element = toElement(n);
        bool childRulesChanged = element->needsStyleRecalc() && element->styleChangeType() == FullStyleChange;
        if ((forceCheckOfNextElementSibling || forceCheckOfAnyElementSibling))
            element->setNeedsStyleRecalc();
        if (shouldRecalcStyle(change, element)) {
            parentPusher.push();
            element->recalcStyle(change);
        }
        forceCheckOfNextElementSibling = childRulesChanged && hasDirectAdjacentRules;
        forceCheckOfAnyElementSibling = forceCheckOfAnyElementSibling || (childRulesChanged && hasIndirectAdjacentRules);
    }

    if (shouldRecalcStyle(change, this))
        updatePseudoElement(AFTER, change);

    clearNeedsStyleRecalc();
    clearChildNeedsStyleRecalc();

    if (hasCustomStyleCallbacks())
        didRecalcStyle(change);
}
