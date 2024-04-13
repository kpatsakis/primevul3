ofputil_packet_in_private_destroy(struct ofputil_packet_in_private *pin)
{
    if (pin) {
        free(pin->stack);
        free(pin->actions);
        free(pin->action_set);
    }
}
