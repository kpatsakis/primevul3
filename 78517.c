static void deprecatedAddMidpoint(LineMidpointState& lineMidpointState, const InlineIterator& midpoint)
{
    if (lineMidpointState.midpoints.size() <= lineMidpointState.numMidpoints)
        lineMidpointState.midpoints.grow(lineMidpointState.numMidpoints + 10);

    InlineIterator* midpoints = lineMidpointState.midpoints.data();
    midpoints[lineMidpointState.numMidpoints++] = midpoint;
}
