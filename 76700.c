bool deleteEmptyDirectory(const String& path)
{
    String filename = path;
    return !!RemoveDirectoryW(filename.charactersWithNullTermination());
}
