static IndentTextOrNot requiresIndent(bool isFirstLine, bool isAfterHardLineBreak, RenderStyle* style)
{
    if (isFirstLine)
        return IndentText;
    if (isAfterHardLineBreak && style->textIndentLine() == TextIndentEachLine)
        return IndentText;

    return DoNotIndentText;
}
