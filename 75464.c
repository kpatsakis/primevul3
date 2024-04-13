void DocumentWriter::addData(const char* bytes, size_t length)
{
    m_parser->appendBytes(this, bytes, length);
}
