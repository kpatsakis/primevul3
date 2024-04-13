static void align_position(AVIOContext *pb,  int64_t offset, uint64_t size)
{
    if (size < INT64_MAX - offset && avio_tell(pb) != offset + size)
        avio_seek(pb, offset + size, SEEK_SET);
}
