static bool isNewLineAtPosition(const Position& position)
{
    Node* textNode = position.containerNode();
    int offset = position.offsetInContainerNode();
    if (!textNode || !textNode->isTextNode() || offset < 0 || offset >= textNode->maxCharacterOffset())
        return false;

    TrackExceptionState es;
    String textAtPosition = toText(textNode)->substringData(offset, 1, es);
    if (es.hadException())
        return false;

    return textAtPosition[0] == '\n';
}
