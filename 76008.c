bool RenderBox::nodeAtPoint(const HitTestRequest& request, HitTestResult& result, const LayoutPoint& pointInContainer, const LayoutPoint& accumulatedOffset, HitTestAction action)
{
    LayoutPoint adjustedLocation = accumulatedOffset + location();

    for (RenderObject* child = lastChild(); child; child = child->previousSibling()) {
        if (!child->hasLayer() && child->nodeAtPoint(request, result, pointInContainer, adjustedLocation, action)) {
            updateHitTestResult(result, pointInContainer - toLayoutSize(adjustedLocation));
            return true;
        }
    }

    LayoutRect boundsRect(adjustedLocation, size());
    if (visibleToHitTesting() && action == HitTestForeground && boundsRect.intersects(result.rectForPoint(pointInContainer))) {
        updateHitTestResult(result, pointInContainer - toLayoutSize(adjustedLocation));
        if (!result.addNodeToRectBasedTestResult(node(), pointInContainer, boundsRect))
            return true;
    }

    return false;
}
