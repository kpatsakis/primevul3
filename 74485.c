static inline String toString(const xmlChar* str, unsigned len)
{
    return UTF8Encoding().decode(reinterpret_cast<const char*>(str), len);
}
