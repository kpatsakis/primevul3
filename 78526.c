static inline void ensureLineBoxInsideIgnoredSpaces(LineMidpointState& lineMidpointState, RenderObject* renderer)
{
    InlineIterator midpoint(0, renderer, 0);
    stopIgnoringSpaces(lineMidpointState, midpoint);
    startIgnoringSpaces(lineMidpointState, midpoint);
}
