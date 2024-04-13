pick_ipc_buffer(unsigned int max)
{
    static unsigned int global_max = 0;

    if (global_max == 0) {
        const char *env = getenv("PCMK_ipc_buffer");

        if (env) {
            int env_max = crm_parse_int(env, "0");

            global_max = (env_max > 0)? QB_MAX(MIN_MSG_SIZE, env_max) : MAX_MSG_SIZE;

        } else {
            global_max = MAX_MSG_SIZE;
        }
    }

    return QB_MAX(max, global_max);
}
