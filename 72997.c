babel_print(netdissect_options *ndo,
            const u_char *cp, u_int length)
{
    ND_PRINT((ndo, "babel"));

    ND_TCHECK2(*cp, 4);

    if(cp[0] != 42) {
        ND_PRINT((ndo, " invalid header"));
        return;
    } else {
        ND_PRINT((ndo, " %d", cp[1]));
    }

    switch(cp[1]) {
    case 2:
        babel_print_v2(ndo, cp, length);
        break;
    default:
        ND_PRINT((ndo, " unknown version"));
        break;
    }

    return;

 trunc:
    ND_PRINT((ndo, " %s", tstr));
    return;
}
