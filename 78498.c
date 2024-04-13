static void checkMidpoints(LineMidpointState& lineMidpointState, InlineIterator& lBreak)
{
    if (lBreak.m_obj && lineMidpointState.numMidpoints && !(lineMidpointState.numMidpoints % 2)) {
        InlineIterator* midpoints = lineMidpointState.midpoints.data();
        InlineIterator& endpoint = midpoints[lineMidpointState.numMidpoints - 2];
        const InlineIterator& startpoint = midpoints[lineMidpointState.numMidpoints - 1];
        InlineIterator currpoint = endpoint;
        while (!currpoint.atEnd() && currpoint != startpoint && currpoint != lBreak)
            currpoint.increment();
        if (currpoint == lBreak) {
            lineMidpointState.numMidpoints--;
            if (endpoint.m_obj->style()->collapseWhiteSpace())
                endpoint.m_pos--;
        }
    }
}
