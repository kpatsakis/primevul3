void RenderBlock::appendRunsForObject(BidiRunList<BidiRun>& runs, int start, int end, RenderObject* obj, InlineBidiResolver& resolver)
{
    if (start > end || shouldSkipCreatingRunsForObject(obj))
        return;

    LineMidpointState& lineMidpointState = resolver.midpointState();
    bool haveNextMidpoint = (lineMidpointState.currentMidpoint < lineMidpointState.numMidpoints);
    InlineIterator nextMidpoint;
    if (haveNextMidpoint)
        nextMidpoint = lineMidpointState.midpoints[lineMidpointState.currentMidpoint];
    if (lineMidpointState.betweenMidpoints) {
        if (!(haveNextMidpoint && nextMidpoint.m_obj == obj))
            return;
        lineMidpointState.betweenMidpoints = false;
        start = nextMidpoint.m_pos;
        lineMidpointState.currentMidpoint++;
        if (start < end)
            return appendRunsForObject(runs, start, end, obj, resolver);
    } else {
        if (!haveNextMidpoint || (obj != nextMidpoint.m_obj)) {
            runs.addRun(createRun(start, end, obj, resolver));
            return;
        }

        if (static_cast<int>(nextMidpoint.m_pos + 1) <= end) {
            lineMidpointState.betweenMidpoints = true;
            lineMidpointState.currentMidpoint++;
            if (nextMidpoint.m_pos != INT_MAX) { // INT_MAX means stop at the object and don't include any of it.
                if (static_cast<int>(nextMidpoint.m_pos + 1) > start)
                    runs.addRun(createRun(start, nextMidpoint.m_pos + 1, obj, resolver));
                return appendRunsForObject(runs, nextMidpoint.m_pos + 1, end, obj, resolver);
            }
        } else
           runs.addRun(createRun(start, end, obj, resolver));
    }
}
