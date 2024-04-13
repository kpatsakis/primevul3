VisiblePosition RenderBox::positionForPoint(const LayoutPoint& point)
{
    if (!firstChild())
        return createVisiblePosition(node() ? firstPositionInOrBeforeNode(node()) : Position());

    if (isTable() && node()) {
        LayoutUnit right = contentWidth() + borderAndPaddingWidth();
        LayoutUnit bottom = contentHeight() + borderAndPaddingHeight();
        
        if (point.x() < 0 || point.x() > right || point.y() < 0 || point.y() > bottom) {
            if (point.x() <= right / 2)
                return createVisiblePosition(firstPositionInOrBeforeNode(node()));
            return createVisiblePosition(lastPositionInOrAfterNode(node()));
        }
    }

    LayoutUnit minDist = numeric_limits<LayoutUnit>::max();
    RenderBox* closestRenderer = 0;
    LayoutPoint adjustedPoint = point;
    if (isTableRow())
        adjustedPoint.moveBy(location());

    for (RenderObject* renderObject = firstChild(); renderObject; renderObject = renderObject->nextSibling()) {
        if ((!renderObject->firstChild() && !renderObject->isInline() && !renderObject->isBlockFlow() )
            || renderObject->style()->visibility() != VISIBLE)
            continue;
        
        if (!renderObject->isBox())
            continue;
        
        RenderBox* renderer = toRenderBox(renderObject);

        LayoutUnit top = renderer->borderTop() + renderer->paddingTop() + (isTableRow() ? 0 : renderer->y());
        LayoutUnit bottom = top + renderer->contentHeight();
        LayoutUnit left = renderer->borderLeft() + renderer->paddingLeft() + (isTableRow() ? 0 : renderer->x());
        LayoutUnit right = left + renderer->contentWidth();
        
        if (point.x() <= right && point.x() >= left && point.y() <= top && point.y() >= bottom) {
            if (renderer->isTableRow())
                return renderer->positionForPoint(point + adjustedPoint - renderer->locationOffset());
            return renderer->positionForPoint(point - renderer->locationOffset());
        }

        LayoutPoint cmp;
        if (point.x() > right) {
            if (point.y() < top)
                cmp = LayoutPoint(right, top);
            else if (point.y() > bottom)
                cmp = LayoutPoint(right, bottom);
            else
                cmp = LayoutPoint(right, point.y());
        } else if (point.x() < left) {
            if (point.y() < top)
                cmp = LayoutPoint(left, top);
            else if (point.y() > bottom)
                cmp = LayoutPoint(left, bottom);
            else
                cmp = LayoutPoint(left, point.y());
        } else {
            if (point.y() < top)
                cmp = LayoutPoint(point.x(), top);
            else
                cmp = LayoutPoint(point.x(), bottom);
        }

        LayoutSize difference = cmp - point;

        LayoutUnit dist = difference.width() * difference.width() + difference.height() * difference.height();
        if (dist < minDist) {
            closestRenderer = renderer;
            minDist = dist;
        }
    }
    
    if (closestRenderer)
        return closestRenderer->positionForPoint(adjustedPoint - closestRenderer->locationOffset());
    
    return createVisiblePosition(firstPositionInOrBeforeNode(node()));
}
