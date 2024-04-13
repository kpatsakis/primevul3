print_browse(netdissect_options *ndo,
             const u_char *param, int paramlen, const u_char *data, int datalen)
{
    const u_char *maxbuf = data + datalen;
    int command;

    ND_TCHECK(data[0]);
    command = data[0];

    smb_fdata(ndo, param, "BROWSE PACKET\n|Param ", param+paramlen, unicodestr);

    switch (command) {
    case 0xF:
	data = smb_fdata(ndo, data,
	    "BROWSE PACKET:\nType=[B] (LocalMasterAnnouncement)\nUpdateCount=[w]\nRes1=[B]\nAnnounceInterval=[d]\nName=[n2]\nMajorVersion=[B]\nMinorVersion=[B]\nServerType=[W]\nElectionVersion=[w]\nBrowserConstant=[w]\n",
	    maxbuf, unicodestr);
	break;

    case 0x1:
	data = smb_fdata(ndo, data,
	    "BROWSE PACKET:\nType=[B] (HostAnnouncement)\nUpdateCount=[w]\nRes1=[B]\nAnnounceInterval=[d]\nName=[n2]\nMajorVersion=[B]\nMinorVersion=[B]\nServerType=[W]\nElectionVersion=[w]\nBrowserConstant=[w]\n",
	    maxbuf, unicodestr);
	break;

    case 0x2:
	data = smb_fdata(ndo, data,
	    "BROWSE PACKET:\nType=[B] (AnnouncementRequest)\nFlags=[B]\nReplySystemName=[S]\n",
	    maxbuf, unicodestr);
	break;

    case 0xc:
	data = smb_fdata(ndo, data,
	    "BROWSE PACKET:\nType=[B] (WorkgroupAnnouncement)\nUpdateCount=[w]\nRes1=[B]\nAnnounceInterval=[d]\nName=[n2]\nMajorVersion=[B]\nMinorVersion=[B]\nServerType=[W]\nCommentPointer=[W]\nServerName=[S]\n",
	    maxbuf, unicodestr);
	break;

    case 0x8:
	data = smb_fdata(ndo, data,
	    "BROWSE PACKET:\nType=[B] (ElectionFrame)\nElectionVersion=[B]\nOSSummary=[W]\nUptime=[(W, W)]\nServerName=[S]\n",
	    maxbuf, unicodestr);
	break;

    case 0xb:
	data = smb_fdata(ndo, data,
	    "BROWSE PACKET:\nType=[B] (BecomeBackupBrowser)\nName=[S]\n",
	    maxbuf, unicodestr);
	break;

    case 0x9:
	data = smb_fdata(ndo, data,
	    "BROWSE PACKET:\nType=[B] (GetBackupList)\nListCount?=[B]\nToken=[W]\n",
	    maxbuf, unicodestr);
	break;

    case 0xa:
	data = smb_fdata(ndo, data,
	    "BROWSE PACKET:\nType=[B] (BackupListResponse)\nServerCount?=[B]\nToken=[W]\n*Name=[S]\n",
	    maxbuf, unicodestr);
	break;

    case 0xd:
	data = smb_fdata(ndo, data,
	    "BROWSE PACKET:\nType=[B] (MasterAnnouncement)\nMasterName=[S]\n",
	    maxbuf, unicodestr);
	break;

    case 0xe:
	data = smb_fdata(ndo, data,
	    "BROWSE PACKET:\nType=[B] (ResetBrowser)\nOptions=[B]\n", maxbuf, unicodestr);
	break;

    default:
	data = smb_fdata(ndo, data, "Unknown Browser Frame ", maxbuf, unicodestr);
	break;
    }
    return;
trunc:
    ND_PRINT((ndo, "%s", tstr));
}
