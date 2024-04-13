void TrailingObjects::updateMidpointsForTrailingBoxes(LineMidpointState& lineMidpointState, const InlineIterator& lBreak, CollapseFirstSpaceOrNot collapseFirstSpace)
{
    if (!m_whitespace)
        return;

    if (lineMidpointState.numMidpoints % 2) {
        int trailingSpaceMidpoint = lineMidpointState.numMidpoints - 1;
        for ( ; trailingSpaceMidpoint > 0 && lineMidpointState.midpoints[trailingSpaceMidpoint].m_obj != m_whitespace; --trailingSpaceMidpoint) { }
        ASSERT(trailingSpaceMidpoint >= 0);
        if (collapseFirstSpace == CollapseFirstSpace)
            lineMidpointState.midpoints[trailingSpaceMidpoint].m_pos--;

        size_t currentMidpoint = trailingSpaceMidpoint + 1;
        for (size_t i = 0; i < m_boxes.size(); ++i) {
            if (currentMidpoint >= lineMidpointState.numMidpoints) {
                ensureLineBoxInsideIgnoredSpaces(lineMidpointState, m_boxes[i]);
            } else {
                ASSERT(lineMidpointState.midpoints[currentMidpoint].m_obj == m_boxes[i]);
                ASSERT(lineMidpointState.midpoints[currentMidpoint + 1].m_obj == m_boxes[i]);
            }
            currentMidpoint += 2;
        }
    } else if (!lBreak.m_obj) {
        ASSERT(m_whitespace->isText());
        ASSERT(collapseFirstSpace == CollapseFirstSpace);
        unsigned length = m_whitespace->textLength();
        unsigned pos = length >= 2 ? length - 2 : INT_MAX;
        InlineIterator endMid(0, m_whitespace, pos);
        startIgnoringSpaces(lineMidpointState, endMid);
        for (size_t i = 0; i < m_boxes.size(); ++i) {
            ensureLineBoxInsideIgnoredSpaces(lineMidpointState, m_boxes[i]);
        }
    }
}
