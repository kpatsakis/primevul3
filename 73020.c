fs_print(netdissect_options *ndo,
         register const u_char *bp, int length)
{
	int fs_op;
	unsigned long i;

	if (length <= (int)sizeof(struct rx_header))
		return;

	if (ndo->ndo_snapend - bp + 1 <= (int)(sizeof(struct rx_header) + sizeof(int32_t))) {
		goto trunc;
	}

	/*
	 * Print out the afs call we're invoking.  The table used here was
	 * gleaned from fsint/afsint.xg
	 */

	fs_op = EXTRACT_32BITS(bp + sizeof(struct rx_header));

	ND_PRINT((ndo, " fs call %s", tok2str(fs_req, "op#%d", fs_op)));

	/*
	 * Print out arguments to some of the AFS calls.  This stuff is
	 * all from afsint.xg
	 */

	bp += sizeof(struct rx_header) + 4;

	/*
	 * Sigh.  This is gross.  Ritchie forgive me.
	 */

	switch (fs_op) {
		case 130:	/* Fetch data */
			FIDOUT();
			ND_PRINT((ndo, " offset"));
			UINTOUT();
			ND_PRINT((ndo, " length"));
			UINTOUT();
			break;
		case 131:	/* Fetch ACL */
		case 132:	/* Fetch Status */
		case 143:	/* Old set lock */
		case 144:	/* Old extend lock */
		case 145:	/* Old release lock */
		case 156:	/* Set lock */
		case 157:	/* Extend lock */
		case 158:	/* Release lock */
			FIDOUT();
			break;
		case 135:	/* Store status */
			FIDOUT();
			STOREATTROUT();
			break;
		case 133:	/* Store data */
			FIDOUT();
			STOREATTROUT();
			ND_PRINT((ndo, " offset"));
			UINTOUT();
			ND_PRINT((ndo, " length"));
			UINTOUT();
			ND_PRINT((ndo, " flen"));
			UINTOUT();
			break;
		case 134:	/* Store ACL */
		{
			char a[AFSOPAQUEMAX+1];
			FIDOUT();
			ND_TCHECK2(bp[0], 4);
			i = EXTRACT_32BITS(bp);
			bp += sizeof(int32_t);
			ND_TCHECK2(bp[0], i);
			i = min(AFSOPAQUEMAX, i);
			strncpy(a, (const char *) bp, i);
			a[i] = '\0';
			acl_print(ndo, (u_char *) a, sizeof(a), (u_char *) a + i);
			break;
		}
		case 137:	/* Create file */
		case 141:	/* MakeDir */
			FIDOUT();
			STROUT(AFSNAMEMAX);
			STOREATTROUT();
			break;
		case 136:	/* Remove file */
		case 142:	/* Remove directory */
			FIDOUT();
			STROUT(AFSNAMEMAX);
			break;
		case 138:	/* Rename file */
			ND_PRINT((ndo, " old"));
			FIDOUT();
			STROUT(AFSNAMEMAX);
			ND_PRINT((ndo, " new"));
			FIDOUT();
			STROUT(AFSNAMEMAX);
			break;
		case 139:	/* Symlink */
			FIDOUT();
			STROUT(AFSNAMEMAX);
			ND_PRINT((ndo, " link to"));
			STROUT(AFSNAMEMAX);
			break;
		case 140:	/* Link */
			FIDOUT();
			STROUT(AFSNAMEMAX);
			ND_PRINT((ndo, " link to"));
			FIDOUT();
			break;
		case 148:	/* Get volume info */
			STROUT(AFSNAMEMAX);
			break;
		case 149:	/* Get volume stats */
		case 150:	/* Set volume stats */
			ND_PRINT((ndo, " volid"));
			UINTOUT();
			break;
		case 154:	/* New get volume info */
			ND_PRINT((ndo, " volname"));
			STROUT(AFSNAMEMAX);
			break;
		case 155:	/* Bulk stat */
		case 65536:     /* Inline bulk stat */
		{
			unsigned long j;
			ND_TCHECK2(bp[0], 4);
			j = EXTRACT_32BITS(bp);
			bp += sizeof(int32_t);

			for (i = 0; i < j; i++) {
				FIDOUT();
				if (i != j - 1)
					ND_PRINT((ndo, ","));
			}
			if (j == 0)
				ND_PRINT((ndo, " <none!>"));
			break;
		}
		case 65537:	/* Fetch data 64 */
			FIDOUT();
			ND_PRINT((ndo, " offset"));
			UINT64OUT();
			ND_PRINT((ndo, " length"));
			UINT64OUT();
			break;
		case 65538:	/* Store data 64 */
			FIDOUT();
			STOREATTROUT();
			ND_PRINT((ndo, " offset"));
			UINT64OUT();
			ND_PRINT((ndo, " length"));
			UINT64OUT();
			ND_PRINT((ndo, " flen"));
			UINT64OUT();
			break;
		case 65541:    /* CallBack rx conn address */
			ND_PRINT((ndo, " addr"));
			UINTOUT();
		default:
			;
	}

	return;

trunc:
	ND_PRINT((ndo, " [|fs]"));
}
