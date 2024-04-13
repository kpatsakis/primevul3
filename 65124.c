static void put_amf_byte(AVIOContext *pb, unsigned char abyte)
{
    avio_w8(pb, abyte);
}
