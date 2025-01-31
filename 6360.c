write_backup_config_file()
{
	std::ostringstream s;

	s << "# This MySQL options file was generated by innobackupex.\n\n"
	  << "# The MySQL server\n"
	  << "[mysqld]\n"
	  << "innodb_checksum_algorithm="
	  << innodb_checksum_algorithm_names[srv_checksum_algorithm] << "\n"
	  << "innodb_log_checksum_algorithm="
	  << innodb_checksum_algorithm_names[srv_log_checksum_algorithm] << "\n"
	  << "innodb_data_file_path=" << innobase_data_file_path << "\n"
	  << "innodb_log_files_in_group=" << srv_n_log_files << "\n"
	  << "innodb_log_file_size=" << innobase_log_file_size << "\n"
	  << "innodb_fast_checksum="
	  << (srv_fast_checksum ? "true" : "false") << "\n"
	  << "innodb_page_size=" << srv_page_size << "\n"
	  << "innodb_log_block_size=" << srv_log_block_size << "\n"
	  << "innodb_undo_directory=" << srv_undo_dir << "\n"
	  << "innodb_undo_tablespaces=" << srv_undo_tablespaces << "\n"
	  << "server_id=" << server_id << "\n"
	  << "redo_log_version=" << redo_log_version << "\n";

	if (innobase_doublewrite_file) {
		s << "innodb_doublewrite_file="
		  << innobase_doublewrite_file << "\n";
	}

	if (innobase_buffer_pool_filename) {
		s << "innodb_buffer_pool_filename="
		  << innobase_buffer_pool_filename << "\n";
	}

	I_List_iterator<i_string> iter(*opt_plugin_load_list_ptr);
	i_string *item;
	while ((item = iter++) != NULL) {
		s << "plugin_load=" << item->ptr << "\n";
	}

	if (server_uuid[0] != 0) {
		s << "server_uuid=" << server_uuid << "\n";
	}
	s << "master_key_id=" << Encryption::master_key_id << "\n";

	return backup_file_print("backup-my.cnf", s.str().c_str(), s.tellp());
}