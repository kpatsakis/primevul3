ofproto_type_run(const char *datapath_type)
{
    const struct ofproto_class *class;
    int error;

    datapath_type = ofproto_normalize_type(datapath_type);
    class = ofproto_class_find__(datapath_type);

    error = class->type_run ? class->type_run(datapath_type) : 0;
    if (error && error != EAGAIN) {
        VLOG_ERR_RL(&rl, "%s: type_run failed (%s)",
                    datapath_type, ovs_strerror(error));
    }
    return error;
}
