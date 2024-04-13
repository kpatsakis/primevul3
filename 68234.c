DetectPostInspectFileFlagsUpdate(Flow *pflow, const SigGroupHead *sgh, uint8_t direction)
{
    /* see if this sgh requires us to consider file storing */
    if (!FileForceFilestore() && (sgh == NULL ||
                sgh->filestore_cnt == 0))
    {
        FileDisableStoring(pflow, direction);
    }
#ifdef HAVE_MAGIC
    /* see if this sgh requires us to consider file magic */
    if (!FileForceMagic() && (sgh == NULL ||
                !(sgh->flags & SIG_GROUP_HEAD_HAVEFILEMAGIC)))
    {
        SCLogDebug("disabling magic for flow");
        FileDisableMagic(pflow, direction);
    }
#endif
    /* see if this sgh requires us to consider file md5 */
    if (!FileForceMd5() && (sgh == NULL ||
                !(sgh->flags & SIG_GROUP_HEAD_HAVEFILEMD5)))
    {
        SCLogDebug("disabling md5 for flow");
        FileDisableMd5(pflow, direction);
    }

    /* see if this sgh requires us to consider file sha1 */
    if (!FileForceSha1() && (sgh == NULL ||
                !(sgh->flags & SIG_GROUP_HEAD_HAVEFILESHA1)))
    {
        SCLogDebug("disabling sha1 for flow");
        FileDisableSha1(pflow, direction);
    }

    /* see if this sgh requires us to consider file sha256 */
    if (!FileForceSha256() && (sgh == NULL ||
                !(sgh->flags & SIG_GROUP_HEAD_HAVEFILESHA256)))
    {
        SCLogDebug("disabling sha256 for flow");
        FileDisableSha256(pflow, direction);
    }

    /* see if this sgh requires us to consider filesize */
    if (sgh == NULL || !(sgh->flags & SIG_GROUP_HEAD_HAVEFILESIZE))
    {
        SCLogDebug("disabling filesize for flow");
        FileDisableFilesize(pflow, direction);
    }
}
