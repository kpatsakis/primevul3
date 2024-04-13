static void clean_html(char *clean, int clean_len, char *dirty)
{
    int i, o;

    for (o = i = 0; o+10 < clean_len && dirty[i];) {
        int len = strspn(dirty+i, "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$-_.+!*(),?/ :;%");
        if (len) {
            if (o + len >= clean_len)
                break;
            memcpy(clean + o, dirty + i, len);
            i += len;
            o += len;
        } else {
            int c = dirty[i++];
            switch (c) {
            case  '&': av_strlcat(clean+o, "&amp;"  , clean_len - o); break;
            case  '<': av_strlcat(clean+o, "&lt;"   , clean_len - o); break;
            case  '>': av_strlcat(clean+o, "&gt;"   , clean_len - o); break;
            case '\'': av_strlcat(clean+o, "&apos;" , clean_len - o); break;
            case '\"': av_strlcat(clean+o, "&quot;" , clean_len - o); break;
            default:   av_strlcat(clean+o, "&#9785;", clean_len - o); break;
            }
            o += strlen(clean+o);
        }
    }
    clean[o] = 0;
}
