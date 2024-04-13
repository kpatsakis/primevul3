void RenderLayerCompositor::computeCompositingRequirements(RenderLayer* ancestorLayer, RenderLayer* layer, OverlapMap* overlapMap, CompositingRecursionData& currentRecursionData, bool& descendantHas3DTransform, Vector<RenderLayer*>& unclippedDescendants)
{
    layer->stackingNode()->updateLayerListsIfNeeded();

    if (overlapMap)
        overlapMap->geometryMap().pushMappingsToAncestor(layer, ancestorLayer);

    layer->setHasCompositingDescendant(false);
    layer->setHasNonCompositedChild(false);

    CompositingReasons reasonsToComposite = CompositingReasonNone;

    CompositingReasons directReasons = directReasonsForCompositing(layer);

    if (currentRecursionData.m_compositingAncestor && currentRecursionData.m_compositingAncestor->renderer()->isVideo())
        directReasons |= CompositingReasonLayerForVideoOverlay;

    if (canBeComposited(layer))
        reasonsToComposite |= directReasons;

    CompositingReasons overlapCompositingReason = currentRecursionData.m_subtreeIsCompositing ? CompositingReasonAssumedOverlap : CompositingReasonNone;

    if (m_renderView->compositorDrivenAcceleratedScrollingEnabled()) {
        Vector<size_t> unclippedDescendantsToRemove;
        for (size_t i = 0; i < unclippedDescendants.size(); i++) {
            RenderLayer* unclippedDescendant = unclippedDescendants.at(i);
            if (unclippedDescendant->renderer()->containingBlock() == layer->renderer()) {
                unclippedDescendantsToRemove.append(i);
                continue;
            }
            if (layer->scrollsWithRespectTo(unclippedDescendant))
                reasonsToComposite |= CompositingReasonAssumedOverlap;
        }

        for (size_t i = 0; i < unclippedDescendantsToRemove.size(); i++)
            unclippedDescendants.remove(unclippedDescendantsToRemove.at(unclippedDescendantsToRemove.size() - i - 1));

        if (reasonsToComposite & CompositingReasonOutOfFlowClipping)
            unclippedDescendants.append(layer);
    }

    bool haveComputedBounds = false;
    IntRect absBounds;
    if (overlapMap && !overlapMap->isEmpty() && currentRecursionData.m_testingOverlap && !requiresCompositingOrSquashing(directReasons)) {
        absBounds = enclosingIntRect(overlapMap->geometryMap().absoluteRect(layer->overlapBounds()));

        if (absBounds.isEmpty())
            absBounds.setSize(IntSize(1, 1));
        haveComputedBounds = true;
        overlapCompositingReason = overlapMap->overlapsLayers(absBounds) ? CompositingReasonOverlap : CompositingReasonNone;
    }

    reasonsToComposite |= overlapCompositingReason;

    if (currentRecursionData.m_mostRecentCompositedLayer && requiresSquashing(reasonsToComposite)
        && layer->scrollsWithRespectTo(currentRecursionData.m_mostRecentCompositedLayer))
        reasonsToComposite |= CompositingReasonOverlapsWithoutSquashingTarget;

    CompositingRecursionData childRecursionData(currentRecursionData);
    childRecursionData.m_subtreeIsCompositing = false;

    bool willBeCompositedOrSquashed = canBeComposited(layer) && requiresCompositingOrSquashing(reasonsToComposite);
    if (willBeCompositedOrSquashed) {
        currentRecursionData.m_subtreeIsCompositing = true;
        childRecursionData.m_compositingAncestor = layer;

        if (overlapMap)
            overlapMap->beginNewOverlapTestingContext();
        childRecursionData.m_testingOverlap = true;
    }

#if !ASSERT_DISABLED
    LayerListMutationDetector mutationChecker(layer->stackingNode());
#endif

    bool anyDescendantHas3DTransform = false;
    bool willHaveForegroundLayer = false;

    if (layer->stackingNode()->isStackingContainer()) {
        RenderLayerStackingNodeIterator iterator(*layer->stackingNode(), NegativeZOrderChildren);
        while (RenderLayerStackingNode* curNode = iterator.next()) {
            computeCompositingRequirements(layer, curNode->layer(), overlapMap, childRecursionData, anyDescendantHas3DTransform, unclippedDescendants);

            if (childRecursionData.m_subtreeIsCompositing) {
                reasonsToComposite |= CompositingReasonNegativeZIndexChildren;

                if (!willBeCompositedOrSquashed) {
                    childRecursionData.m_compositingAncestor = layer;
                    overlapMap->beginNewOverlapTestingContext();
                    willBeCompositedOrSquashed = true;
                    willHaveForegroundLayer = true;

                    if (overlapMap) {
                        overlapMap->geometryMap().pushMappingsToAncestor(curNode->layer(), layer);
                        IntRect childAbsBounds = enclosingIntRect(overlapMap->geometryMap().absoluteRect(curNode->layer()->overlapBounds()));
                        bool boundsComputed = true;
                        overlapMap->beginNewOverlapTestingContext();
                        addToOverlapMap(*overlapMap, curNode->layer(), childAbsBounds, boundsComputed);
                        overlapMap->finishCurrentOverlapTestingContext();
                        overlapMap->geometryMap().popMappingsToAncestor(layer);
                    }
                }
            }
        }
    }

    if (overlapMap && willHaveForegroundLayer) {
        ASSERT(willBeCompositedOrSquashed);
        overlapMap->finishCurrentOverlapTestingContext();
        overlapMap->beginNewOverlapTestingContext();
        childRecursionData.m_testingOverlap = true;
    }

    if (requiresCompositing(reasonsToComposite)) {
        currentRecursionData.m_mostRecentCompositedLayer = layer;
        childRecursionData.m_mostRecentCompositedLayer = layer;
    }

    RenderLayerStackingNodeIterator iterator(*layer->stackingNode(), NormalFlowChildren | PositiveZOrderChildren);
    while (RenderLayerStackingNode* curNode = iterator.next())
        computeCompositingRequirements(layer, curNode->layer(), overlapMap, childRecursionData, anyDescendantHas3DTransform, unclippedDescendants);

    currentRecursionData.m_mostRecentCompositedLayer = childRecursionData.m_mostRecentCompositedLayer;


    if (layer->isRootLayer()) {
        if (inCompositingMode() && m_hasAcceleratedCompositing)
            willBeCompositedOrSquashed = true;
    }

    if (overlapMap && childRecursionData.m_compositingAncestor && !childRecursionData.m_compositingAncestor->isRootLayer())
        addToOverlapMap(*overlapMap, layer, absBounds, haveComputedBounds);

    if (layer->stackingNode()->isStackingContext()) {
        layer->setShouldIsolateCompositedDescendants(childRecursionData.m_hasUnisolatedCompositedBlendingDescendant);
    } else {
        layer->setShouldIsolateCompositedDescendants(false);
        currentRecursionData.m_hasUnisolatedCompositedBlendingDescendant = childRecursionData.m_hasUnisolatedCompositedBlendingDescendant;
    }

    CompositingReasons subtreeCompositingReasons = subtreeReasonsForCompositing(layer->renderer(), childRecursionData.m_subtreeIsCompositing, anyDescendantHas3DTransform);
    reasonsToComposite |= subtreeCompositingReasons;
    if (!willBeCompositedOrSquashed && canBeComposited(layer) && requiresCompositingOrSquashing(subtreeCompositingReasons)) {
        childRecursionData.m_compositingAncestor = layer;
        if (overlapMap) {
            overlapMap->beginNewOverlapTestingContext();
            addToOverlapMapRecursive(*overlapMap, layer);
        }
        willBeCompositedOrSquashed = true;
    }

    if (layer->reflectionInfo()) {
        CompositingReasons reflectionCompositingReason = willBeCompositedOrSquashed ? CompositingReasonReflectionOfCompositedParent : CompositingReasonNone;
        layer->reflectionInfo()->reflectionLayer()->setCompositingReasons(layer->reflectionInfo()->reflectionLayer()->compositingReasons() | reflectionCompositingReason);
    }

    if (childRecursionData.m_subtreeIsCompositing)
        currentRecursionData.m_subtreeIsCompositing = true;

    if (willBeCompositedOrSquashed && layer->blendInfo().hasBlendMode())
        currentRecursionData.m_hasUnisolatedCompositedBlendingDescendant = true;

    layer->setHasCompositingDescendant(childRecursionData.m_subtreeIsCompositing);

    bool isCompositedClippingLayer = canBeComposited(layer) && (reasonsToComposite & CompositingReasonClipsCompositingDescendants);
    if ((!childRecursionData.m_testingOverlap && !isCompositedClippingLayer) || isRunningAcceleratedTransformAnimation(layer->renderer()))
        currentRecursionData.m_testingOverlap = false;

    if (overlapMap && childRecursionData.m_compositingAncestor == layer && !layer->isRootLayer())
        overlapMap->finishCurrentOverlapTestingContext();

    if (layer->isRootLayer()) {
        if (childRecursionData.m_subtreeIsCompositing || requiresCompositingOrSquashing(reasonsToComposite) || m_forceCompositingMode) {
            willBeCompositedOrSquashed = true;
            reasonsToComposite |= CompositingReasonRoot;
        } else {
            enableCompositingMode(false);
            willBeCompositedOrSquashed = false;
            reasonsToComposite = CompositingReasonNone;
        }
    }

    if (requiresCompositing(reasonsToComposite))
        currentRecursionData.m_mostRecentCompositedLayer = layer;

    layer->setCompositingReasons(reasonsToComposite);

    if (!willBeCompositedOrSquashed && layer->parent())
        layer->parent()->setHasNonCompositedChild(true);

    descendantHas3DTransform |= anyDescendantHas3DTransform || layer->has3DTransform();

    if (overlapMap)
        overlapMap->geometryMap().popMappingsToAncestor(ancestorLayer);
}
