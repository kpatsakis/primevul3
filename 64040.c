ofpacts_format(const struct ofpact *ofpacts, size_t ofpacts_len,
               struct ds *string)
{
    if (!ofpacts_len) {
        ds_put_format(string, "%sdrop%s", colors.drop, colors.end);
    } else {
        const struct ofpact *a;

        OFPACT_FOR_EACH (a, ofpacts, ofpacts_len) {
            if (a != ofpacts) {
                ds_put_char(string, ',');
            }

            ofpact_format(a, string);
        }
    }
}
