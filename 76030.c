void RenderBox::removeFloatingOrPositionedChildFromBlockLists()
{
    ASSERT(isFloatingOrPositioned());

    if (documentBeingDestroyed())
        return;

    if (isFloating()) {
        RenderBlock* parentBlock = 0;
        for (RenderObject* curr = parent(); curr && !curr->isRenderView(); curr = curr->parent()) {
            if (curr->isRenderBlock()) {
                RenderBlock* currBlock = toRenderBlock(curr);
                if (!parentBlock || currBlock->containsFloat(this))
                    parentBlock = currBlock;
            }
        }

        if (parentBlock) {
            RenderObject* parent = parentBlock->parent();
            if (parent && parent->isDeprecatedFlexibleBox())
                parentBlock = toRenderBlock(parent);

            parentBlock->markAllDescendantsWithFloatsForLayout(this, false);
        }
    }

    if (isPositioned()) {
        for (RenderObject* curr = parent(); curr; curr = curr->parent()) {
            if (curr->isRenderBlock())
                toRenderBlock(curr)->removePositionedObject(this);
        }
    }
}
