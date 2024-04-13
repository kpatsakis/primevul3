static inline String toString(const xmlChar* str)
{
    if (!str)
        return String();
    
    return UTF8Encoding().decode(reinterpret_cast<const char*>(str), strlen(reinterpret_cast<const char*>(str)));
}
