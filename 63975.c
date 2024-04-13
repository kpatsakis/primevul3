format_SAMPLE(const struct ofpact_sample *a, struct ds *s)
{
    ds_put_format(s, "%ssample(%s%sprobability=%s%"PRIu16
                  ",%scollector_set_id=%s%"PRIu32
                  ",%sobs_domain_id=%s%"PRIu32
                  ",%sobs_point_id=%s%"PRIu32,
                  colors.paren, colors.end,
                  colors.param, colors.end, a->probability,
                  colors.param, colors.end, a->collector_set_id,
                  colors.param, colors.end, a->obs_domain_id,
                  colors.param, colors.end, a->obs_point_id);
    if (a->sampling_port != OFPP_NONE) {
        ds_put_format(s, ",%ssampling_port=%s%"PRIu32,
                      colors.param, colors.end, a->sampling_port);
    }
    if (a->direction == NX_ACTION_SAMPLE_INGRESS) {
        ds_put_format(s, ",%singress%s", colors.param, colors.end);
    } else if (a->direction == NX_ACTION_SAMPLE_EGRESS) {
        ds_put_format(s, ",%segress%s", colors.param, colors.end);
    }
    ds_put_format(s, "%s)%s", colors.paren, colors.end);
}
