static inline void startIgnoringSpaces(LineMidpointState& lineMidpointState, const InlineIterator& midpoint)
{
    ASSERT(!(lineMidpointState.numMidpoints % 2));
    deprecatedAddMidpoint(lineMidpointState, midpoint);
}
