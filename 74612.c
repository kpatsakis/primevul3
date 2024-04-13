static void pageCacheLog(const String& prefix, const String& message)
{
    LOG(PageCache, "%s%s", prefix.utf8().data(), message.utf8().data());
}
