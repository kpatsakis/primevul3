InlineBox* InlineFlowBox::firstLeafChild() const
{
    InlineBox* leaf = 0;
    for (InlineBox* child = firstChild(); child && !leaf; child = child->nextOnLine())
        leaf = child->isLeaf() ? child : static_cast<InlineFlowBox*>(child)->firstLeafChild();
    return leaf;
}
