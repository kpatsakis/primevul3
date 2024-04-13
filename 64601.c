ofputil_normalize_match(struct match *match)
{
    ofputil_normalize_match__(match, true);
}
