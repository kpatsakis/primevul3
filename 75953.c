IntRect RenderBox::clipRect(const IntPoint& location)
{
    IntRect clipRect(location, size());
    if (!style()->clipLeft().isAuto()) {
        int c = style()->clipLeft().calcValue(width());
        clipRect.move(c, 0);
        clipRect.contract(c, 0);
    }

    if (!style()->clipRight().isAuto())
        clipRect.contract(width() - style()->clipRight().calcValue(width()), 0);

    if (!style()->clipTop().isAuto()) {
        int c = style()->clipTop().calcValue(height());
        clipRect.move(0, c);
        clipRect.contract(0, c);
    }

    if (!style()->clipBottom().isAuto())
        clipRect.contract(0, height() - style()->clipBottom().calcValue(height()));

    return clipRect;
}
