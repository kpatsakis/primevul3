void XMLTokenizer::stopParsing()
{
    Tokenizer::stopParsing();
    xmlStopParser(m_context);
}
