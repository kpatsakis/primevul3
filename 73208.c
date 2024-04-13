static int channel_close (lua_State *L, int status, lua_KContext ctx) {
    int rc;
    LIBSSH2_CHANNEL **channel = (LIBSSH2_CHANNEL **) lua_touserdata(L, 2);

    while ((rc = libssh2_channel_close(*channel)) == LIBSSH2_ERROR_EAGAIN) {
        luaL_getmetafield(L, 1, "filter");
        lua_pushvalue(L, 1);
        lua_callk(L, 1, 0, 0, channel_close);
    }
    if (rc != 0)
        return luaL_error(L, "Error closing channel");;

    return 0;
}
