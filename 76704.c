bool getFileModificationTime(const String& path, time_t& time)
{
    WIN32_FIND_DATAW findData;
    if (!getFindData(path, findData))
        return false;

    getFileModificationTimeFromFindData(findData, time);
    return true;
}
