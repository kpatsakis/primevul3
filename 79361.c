String CharacterIterator::string(int numChars)
{
    Vector<UChar> result;
    result.reserveInitialCapacity(numChars);
    while (numChars > 0 && !atEnd()) {
        int runSize = min(numChars, length());
        result.append(characters(), runSize);
        numChars -= runSize;
        advance(runSize);
    }
    return String::adopt(result);
}
