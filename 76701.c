bool deleteFile(const String& path)
{
    String filename = path;
    return !!DeleteFileW(filename.charactersWithNullTermination());
}
