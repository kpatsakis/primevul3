bad_group_cmd(enum ofp15_group_mod_command cmd)
{
    const char *opt_version;
    const char *version;
    const char *cmd_str;

    switch (cmd) {
    case OFPGC15_ADD:
    case OFPGC15_MODIFY:
    case OFPGC15_ADD_OR_MOD:
    case OFPGC15_DELETE:
        version = "1.1";
        opt_version = "11";
        break;

    case OFPGC15_INSERT_BUCKET:
    case OFPGC15_REMOVE_BUCKET:
        version = "1.5";
        opt_version = "15";
        break;

    default:
        OVS_NOT_REACHED();
    }

    switch (cmd) {
    case OFPGC15_ADD:
        cmd_str = "add-group";
        break;

    case OFPGC15_MODIFY:
    case OFPGC15_ADD_OR_MOD:
        cmd_str = "mod-group";
        break;

    case OFPGC15_DELETE:
        cmd_str = "del-group";
        break;

    case OFPGC15_INSERT_BUCKET:
        cmd_str = "insert-bucket";
        break;

    case OFPGC15_REMOVE_BUCKET:
        cmd_str = "remove-bucket";
        break;

    default:
        OVS_NOT_REACHED();
    }

    ovs_fatal(0, "%s needs OpenFlow %s or later (\'-O OpenFlow%s\')",
              cmd_str, version, opt_version);

}
