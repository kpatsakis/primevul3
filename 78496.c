void RenderBlock::checkFloatsInCleanLine(RootInlineBox* line, Vector<FloatWithRect>& floats, size_t& floatIndex, bool& encounteredNewFloat, bool& dirtiedByFloat)
{
    Vector<RenderBox*>* cleanLineFloats = line->floatsPtr();
    if (!cleanLineFloats)
        return;

    Vector<RenderBox*>::iterator end = cleanLineFloats->end();
    for (Vector<RenderBox*>::iterator it = cleanLineFloats->begin(); it != end; ++it) {
        RenderBox* floatingBox = *it;
        floatingBox->layoutIfNeeded();
        LayoutSize newSize(floatingBox->width() + floatingBox->marginWidth(), floatingBox->height() + floatingBox->marginHeight());
        if (floats[floatIndex].object != floatingBox) {
            encounteredNewFloat = true;
            return;
        }

        if (floats[floatIndex].rect.size() != newSize) {
            LayoutUnit floatTop = isHorizontalWritingMode() ? floats[floatIndex].rect.y() : floats[floatIndex].rect.x();
            LayoutUnit floatHeight = isHorizontalWritingMode() ? max(floats[floatIndex].rect.height(), newSize.height())
                                                                 : max(floats[floatIndex].rect.width(), newSize.width());
            floatHeight = min(floatHeight, LayoutUnit::max() - floatTop);
            line->markDirty();
            markLinesDirtyInBlockRange(line->lineBottomWithLeading(), floatTop + floatHeight, line);
            floats[floatIndex].rect.setSize(newSize);
            dirtiedByFloat = true;
        }
        floatIndex++;
    }
}
