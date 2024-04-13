bool ContainerNode::getUpperLeftCorner(FloatPoint& point) const
{
    if (!renderer())
        return false;
    RenderObject *o = renderer();
    RenderObject *p = o;

    if (!o->isInline() || o->isReplaced()) {
        point = o->localToAbsolute(FloatPoint(), false, true);
        return true;
    }

    while (o) {
        p = o;
        if (o->firstChild())
            o = o->firstChild();
        else if (o->nextSibling())
            o = o->nextSibling();
        else {
            RenderObject *next = 0;
            while (!next && o->parent()) {
                o = o->parent();
                next = o->nextSibling();
            }
            o = next;

            if (!o)
                break;
        }
        ASSERT(o);

        if (!o->isInline() || o->isReplaced()) {
            point = o->localToAbsolute(FloatPoint(), false, true);
            return true;
        }

        if (p->node() && p->node() == this && o->isText() && !o->isBR() && !toRenderText(o)->firstTextBox()) {
        } else if ((o->isText() && !o->isBR()) || o->isReplaced()) {
            point = FloatPoint();
            if (o->isText() && toRenderText(o)->firstTextBox()) {
                point.move(toRenderText(o)->linesBoundingBox().x(),
                           toRenderText(o)->firstTextBox()->root()->lineTop());
            } else if (o->isBox()) {
                RenderBox* box = toRenderBox(o);
                point.moveBy(box->location());
            }
            point = o->container()->localToAbsolute(point, false, true);
            return true;
        }
    }
    
    if (!o && document()->view()) {
        point = FloatPoint(0, document()->view()->contentsHeight());
        return true;
    }
    return false;
}
