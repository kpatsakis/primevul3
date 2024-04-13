static int gc (lua_State *L) {
    struct ssh_userdata *sshu = NULL;

    sshu = (struct ssh_userdata *) nseU_checkudata(L, 1, SSH2_UDATA, "ssh2");
    if (!sshu) { return 0; }
    if (sshu) {

        if (sshu->session != NULL) {
            if (libssh2_session_free(sshu->session) < 0) {
            }
            sshu->session = NULL;
        }
    }

#ifdef WIN32
    closesocket(sshu->sp[0]);
    closesocket(sshu->sp[1]);
#else
    close(sshu->sp[0]);
    close(sshu->sp[1]);
#endif

    return 0;
}
