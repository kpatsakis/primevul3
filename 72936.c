smbfind(int id, const struct smbfns *list)
{
    int sindex;

    for (sindex = 0; list[sindex].name; sindex++)
	if (list[sindex].id == id)
	    return(&list[sindex]);

    return(&list[0]);
}
