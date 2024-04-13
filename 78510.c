static inline InlineBox* createInlineBoxForRenderer(RenderObject* obj, bool isRootLineBox, bool isOnlyRun = false)
{
    if (isRootLineBox)
        return toRenderBlock(obj)->createAndAppendRootInlineBox();

    if (obj->isText()) {
        InlineTextBox* textBox = toRenderText(obj)->createInlineTextBox();
        if (obj->isBR())
            textBox->setIsText(isOnlyRun || obj->document().inNoQuirksMode());
        return textBox;
    }

    if (obj->isBox())
        return toRenderBox(obj)->createInlineBox();

    return toRenderInline(obj)->createAndAppendInlineFlowBox();
}
