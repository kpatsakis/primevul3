static void put_timestamp(AVIOContext *pb, int64_t ts) {
    avio_wb24(pb, ts & 0xFFFFFF);
    avio_w8(pb, (ts >> 24) & 0x7F);
}
