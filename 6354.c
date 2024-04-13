mdl_unlock_all()
{
	msg_ts("Unlocking MDL for all tables\n");
	if (mdl_con != NULL) {
		xb_mysql_query(mdl_con, "COMMIT", false, true);
		mysql_close(mdl_con);
	}
}