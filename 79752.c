static inline unsigned computeLengthForSubmission(const String& text)
{
    return text.length() + numberOfLineBreaks(text);
}
