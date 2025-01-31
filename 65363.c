static int mov_write_vpcc_tag(AVFormatContext *s, AVIOContext *pb, MOVTrack *track)
{
    int64_t pos = avio_tell(pb);

    avio_wb32(pb, 0);
    ffio_wfourcc(pb, "vpcC");
    avio_w8(pb, 1); /* version */
    avio_wb24(pb, 0); /* flags */
    ff_isom_write_vpcc(s, pb, track->par);
    return update_size(pb, pos);
}
