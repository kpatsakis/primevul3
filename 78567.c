void RenderBlock::repaintDirtyFloats(Vector<FloatWithRect>& floats)
{
    size_t floatCount = floats.size();
    for (size_t i = 0; i < floatCount; ++i) {
        if (!floats[i].everHadLayout) {
            RenderBox* f = floats[i].object;
            if (!f->x() && !f->y() && f->checkForRepaintDuringLayout())
                f->repaint();
        }
    }
}
