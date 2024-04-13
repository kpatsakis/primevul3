		void CWebServer::ReloadCustomSwitchIcons()
		{
			m_custom_light_icons.clear();
			m_custom_light_icons_lookup.clear();
			std::string sLine = "";

			std::ifstream infile;
			std::string switchlightsfile = szWWWFolder + "/switch_icons.txt";
			infile.open(switchlightsfile.c_str());
			if (infile.is_open())
			{
				int index = 0;
				while (!infile.eof())
				{
					getline(infile, sLine);
					if (sLine.size() != 0)
					{
						std::vector<std::string> results;
						StringSplit(sLine, ";", results);
						if (results.size() == 3)
						{
							_tCustomIcon cImage;
							cImage.idx = index++;
							cImage.RootFile = results[0];
							cImage.Title = results[1];
							cImage.Description = results[2];
							m_custom_light_icons.push_back(cImage);
							m_custom_light_icons_lookup[cImage.idx] = m_custom_light_icons.size() - 1;
						}
					}
				}
				infile.close();
			}
			std::vector<std::vector<std::string> > result;
			result = m_sql.safe_query("SELECT ID,Base,Name,Description FROM CustomImages");
			if (!result.empty())
			{
				int ii = 0;
				for (const auto & itt : result)
				{
					std::vector<std::string> sd = itt;

					int ID = atoi(sd[0].c_str());

					_tCustomIcon cImage;
					cImage.idx = 100 + ID;
					cImage.RootFile = sd[1];
					cImage.Title = sd[2];
					cImage.Description = sd[3];

					std::string IconFile16 = cImage.RootFile + ".png";
					std::string IconFile48On = cImage.RootFile + "48_On.png";
					std::string IconFile48Off = cImage.RootFile + "48_Off.png";

					std::map<std::string, std::string> _dbImageFiles;
					_dbImageFiles["IconSmall"] = szWWWFolder + "/images/" + IconFile16;
					_dbImageFiles["IconOn"] = szWWWFolder + "/images/" + IconFile48On;
					_dbImageFiles["IconOff"] = szWWWFolder + "/images/" + IconFile48Off;

					for (const auto & iItt : _dbImageFiles)
					{
						std::string TableField = iItt.first;
						std::string IconFile = iItt.second;

						if (!file_exist(IconFile.c_str()))
						{
							std::vector<std::vector<std::string> > result2;
							result2 = m_sql.safe_queryBlob("SELECT %s FROM CustomImages WHERE ID=%d", TableField.c_str(), ID);
							if (!result2.empty())
							{
								std::ofstream file;
								file.open(IconFile.c_str(), std::ios::out | std::ios::binary);
								if (!file.is_open())
									return;

								file << result2[0][0];
								file.close();
							}
						}
					}

					m_custom_light_icons.push_back(cImage);
					m_custom_light_icons_lookup[cImage.idx] = m_custom_light_icons.size() - 1;
					ii++;
				}
			}
		}
