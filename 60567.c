static int find_stream_in_feed(FFServerStream *feed, AVCodecParameters *codec,
                               int bit_rate)
{
    int i;
    int best_bitrate = 100000000;
    int best = -1;

    for (i = 0; i < feed->nb_streams; i++) {
        AVCodecParameters *feed_codec = feed->streams[i]->codecpar;

        if (feed_codec->codec_id != codec->codec_id ||
            feed_codec->sample_rate != codec->sample_rate ||
            feed_codec->width != codec->width ||
            feed_codec->height != codec->height)
            continue;

        /* Potential stream */

        /* We want the fastest stream less than bit_rate, or the slowest
         * faster than bit_rate
         */

        if (feed_codec->bit_rate <= bit_rate) {
            if (best_bitrate > bit_rate ||
                feed_codec->bit_rate > best_bitrate) {
                best_bitrate = feed_codec->bit_rate;
                best = i;
            }
            continue;
        }
        if (feed_codec->bit_rate < best_bitrate) {
            best_bitrate = feed_codec->bit_rate;
            best = i;
        }
    }
    return best;
}
