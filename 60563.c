static int64_t ffm_read_write_index(int fd)
{
    uint8_t buf[8];

    if (lseek(fd, 8, SEEK_SET) < 0)
        return AVERROR(EIO);
    if (read(fd, buf, 8) != 8)
        return AVERROR(EIO);
    return AV_RB64(buf);
}
