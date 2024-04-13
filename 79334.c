String plainText(const Range* r, TextIteratorBehavior defaultBehavior, bool isDisplayString)
{
    static const unsigned cMaxSegmentSize = 1 << 15;

    unsigned bufferLength = 0;
    StringBuilder builder;
    builder.reserveCapacity(cMaxSegmentSize);
    TextIteratorBehavior behavior = defaultBehavior;
    if (!isDisplayString)
        behavior = static_cast<TextIteratorBehavior>(behavior | TextIteratorEmitsTextsWithoutTranscoding);
    
    for (TextIterator it(r, behavior); !it.atEnd(); it.advance()) {
        if (builder.capacity() < builder.length() + it.length())
            builder.reserveCapacity(builder.capacity() + cMaxSegmentSize);

        it.appendTextToStringBuilder(builder);
        bufferLength += it.length();
    }

    if (!bufferLength)
        return emptyString();

    String result = builder.toString();

    if (isDisplayString && r->ownerDocument())
        r->ownerDocument()->displayStringModifiedByEncoding(result);

    return result;
}
