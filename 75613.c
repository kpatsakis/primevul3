bool ContainerNode::getLowerRightCorner(FloatPoint& point) const
{
    if (!renderer())
        return false;

    RenderObject* o = renderer();
    if (!o->isInline() || o->isReplaced()) {
        RenderBox* box = toRenderBox(o);
        point = o->localToAbsolute(LayoutPoint(box->size()), false, true);
        return true;
    }

    while (o) {
        if (o->lastChild())
            o = o->lastChild();
        else if (o->previousSibling())
            o = o->previousSibling();
        else {
            RenderObject* prev = 0;
            while (!prev) {
                o = o->parent();
                if (!o)
                    return false;
                prev = o->previousSibling();
            }
            o = prev;
        }
        ASSERT(o);
        if (o->isText() || o->isReplaced()) {
            point = FloatPoint();
            if (o->isText()) {
                RenderText* text = toRenderText(o);
                IntRect linesBox = text->linesBoundingBox();
                if (!linesBox.maxX() && !linesBox.maxY())
                    continue;
                point.moveBy(linesBox.maxXMaxYCorner());
            } else {
                RenderBox* box = toRenderBox(o);
                point.moveBy(box->frameRect().maxXMaxYCorner());
            }
            point = o->container()->localToAbsolute(point, false, true);
            return true;
        }
    }
    return true;
}
