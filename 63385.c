hfs_attrTypeName(uint32_t typeNum)
{
    switch (typeNum) {
    case TSK_FS_ATTR_TYPE_HFS_DEFAULT:
        return "DFLT";
    case TSK_FS_ATTR_TYPE_HFS_DATA:
        return "DATA";
    case TSK_FS_ATTR_TYPE_HFS_EXT_ATTR:
        return "ExATTR";
    case TSK_FS_ATTR_TYPE_HFS_COMP_REC:
        return "CMPF";
    case TSK_FS_ATTR_TYPE_HFS_RSRC:
        return "RSRC";
    default:
        return "UNKN";
    }
}
