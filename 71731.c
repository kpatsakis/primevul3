		void CWebServer::HandleCommand(const std::string &cparam, WebEmSession & session, const request& req, Json::Value &root)
		{
			std::map < std::string, webserver_response_function >::iterator pf = m_webcommands.find(cparam);
			if (pf != m_webcommands.end())
			{
				pf->second(session, req, root);
				return;
			}

			std::vector<std::vector<std::string> > result;
			char szTmp[300];

			bool bHaveUser = (session.username != "");
			int iUser = -1;
			if (bHaveUser)
			{
				iUser = FindUser(session.username.c_str());
			}

			if (cparam == "deleteallsubdevices")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				root["status"] = "OK";
				root["title"] = "DeleteAllSubDevices";
				result = m_sql.safe_query("DELETE FROM LightSubDevices WHERE (ParentID == '%q')", idx.c_str());
			}
			else if (cparam == "deletesubdevice")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				root["status"] = "OK";
				root["title"] = "DeleteSubDevice";
				result = m_sql.safe_query("DELETE FROM LightSubDevices WHERE (ID == '%q')", idx.c_str());
			}
			else if (cparam == "addsubdevice")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				std::string subidx = request::findValue(&req, "subidx");
				if ((idx.empty()) || (subidx.empty()))
					return;
				if (idx == subidx)
					return;

				result = m_sql.safe_query("SELECT ID FROM LightSubDevices WHERE (DeviceRowID=='%q') AND (ParentID =='%q')",
					subidx.c_str(), idx.c_str());
				if (result.empty())
				{
					root["status"] = "OK";
					root["title"] = "AddSubDevice";
					result = m_sql.safe_query(
						"INSERT INTO LightSubDevices (DeviceRowID, ParentID) VALUES ('%q','%q')",
						subidx.c_str(),
						idx.c_str()
					);
				}
			}
			else if (cparam == "addscenedevice")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				std::string devidx = request::findValue(&req, "devidx");
				std::string isscene = request::findValue(&req, "isscene");
				std::string scommand = request::findValue(&req, "command");
				int ondelay = atoi(request::findValue(&req, "ondelay").c_str());
				int offdelay = atoi(request::findValue(&req, "offdelay").c_str());

				if (
					(idx.empty()) ||
					(devidx.empty()) ||
					(isscene.empty())
					)
					return;
				int level = -1;
				if (request::hasValue(&req, "level"))
					level = atoi(request::findValue(&req, "level").c_str());
				std::string color = _tColor(request::findValue(&req, "color")).toJSONString(); //Parse the color to detect incorrectly formatted color data

				unsigned char command = 0;
				result = m_sql.safe_query("SELECT HardwareID, DeviceID, Unit, Type, SubType, SwitchType, Options FROM DeviceStatus WHERE (ID=='%q')",
					devidx.c_str());
				if (!result.empty())
				{
					int dType = atoi(result[0][3].c_str());
					int sType = atoi(result[0][4].c_str());
					_eSwitchType switchtype = (_eSwitchType)atoi(result[0][5].c_str());
					std::map<std::string, std::string> options = m_sql.BuildDeviceOptions(result[0][6].c_str());
					GetLightCommand(dType, sType, switchtype, scommand, command, options);
				}

				result = m_sql.safe_query("SELECT Activators, SceneType FROM Scenes WHERE (ID=='%q')", idx.c_str());
				if (!result.empty())
				{
					int SceneType = atoi(result[0][1].c_str());

					std::vector<std::string> arrayActivators;
					StringSplit(result[0][0], ";", arrayActivators);
					for (const auto & ittAct : arrayActivators)
					{
						std::string sCodeCmd = ittAct;

						std::vector<std::string> arrayCode;
						StringSplit(sCodeCmd, ":", arrayCode);

						std::string sID = arrayCode[0];
						std::string sCode = "";
						if (arrayCode.size() == 2)
						{
							sCode = arrayCode[1];
						}

						if (sID == devidx)
						{
							return; //Group does not work with separate codes, so already there
						}
					}
				}
				if (isscene == "true") {
					result = m_sql.safe_query("SELECT ID FROM SceneDevices WHERE (DeviceRowID=='%q') AND (SceneRowID =='%q') AND (OnDelay == %d) AND (OffDelay == %d) AND (Cmd == %d)",
						devidx.c_str(), idx.c_str(), ondelay, offdelay, command);
				}
				else {
					result = m_sql.safe_query("SELECT ID FROM SceneDevices WHERE (DeviceRowID=='%q') AND (SceneRowID =='%q') AND (OnDelay == %d)",
						devidx.c_str(), idx.c_str(), ondelay);
				}
				if (result.empty())
				{
					root["status"] = "OK";
					root["title"] = "AddSceneDevice";
					if (isscene == "true")
					{
						m_sql.safe_query(
							"INSERT INTO SceneDevices (DeviceRowID, SceneRowID, Cmd, Level, Color, OnDelay, OffDelay) VALUES ('%q','%q',%d,%d,'%q',%d,%d)",
							devidx.c_str(),
							idx.c_str(),
							command,
							level,
							color.c_str(),
							ondelay,
							offdelay
						);
					}
					else
					{
						m_sql.safe_query(
							"INSERT INTO SceneDevices (DeviceRowID, SceneRowID, Level, Color, OnDelay, OffDelay) VALUES ('%q','%q',%d,'%q',%d,%d)",
							devidx.c_str(),
							idx.c_str(),
							level,
							color.c_str(),
							ondelay,
							offdelay
						);
					}
					if (m_sql.m_bEnableEventSystem)
						m_mainworker.m_eventsystem.GetCurrentScenesGroups();
				}
			}
			else if (cparam == "updatescenedevice")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				std::string devidx = request::findValue(&req, "devidx");
				std::string scommand = request::findValue(&req, "command");
				int ondelay = atoi(request::findValue(&req, "ondelay").c_str());
				int offdelay = atoi(request::findValue(&req, "offdelay").c_str());

				if (
					(idx.empty()) ||
					(devidx.empty())
					)
					return;

				unsigned char command = 0;

				result = m_sql.safe_query("SELECT HardwareID, DeviceID, Unit, Type, SubType, SwitchType, Options FROM DeviceStatus WHERE (ID=='%q')",
					devidx.c_str());
				if (!result.empty())
				{
					int dType = atoi(result[0][3].c_str());
					int sType = atoi(result[0][4].c_str());
					_eSwitchType switchtype = (_eSwitchType)atoi(result[0][5].c_str());
					std::map<std::string, std::string> options = m_sql.BuildDeviceOptions(result[0][6].c_str());
					GetLightCommand(dType, sType, switchtype, scommand, command, options);
				}
				int level = -1;
				if (request::hasValue(&req, "level"))
					level = atoi(request::findValue(&req, "level").c_str());
				std::string color = _tColor(request::findValue(&req, "color")).toJSONString(); //Parse the color to detect incorrectly formatted color data
				root["status"] = "OK";
				root["title"] = "UpdateSceneDevice";
				result = m_sql.safe_query(
					"UPDATE SceneDevices SET Cmd=%d, Level=%d, Color='%q', OnDelay=%d, OffDelay=%d  WHERE (ID == '%q')",
					command, level, color.c_str(), ondelay, offdelay, idx.c_str());
			}
			else if (cparam == "deletescenedevice")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				root["status"] = "OK";
				root["title"] = "DeleteSceneDevice";
				m_sql.safe_query("DELETE FROM SceneDevices WHERE (ID == '%q')", idx.c_str());
				m_sql.safe_query("DELETE FROM CamerasActiveDevices WHERE (DevSceneType==1) AND (DevSceneRowID == '%q')", idx.c_str());
				if (m_sql.m_bEnableEventSystem)
					m_mainworker.m_eventsystem.GetCurrentScenesGroups();
			}
			else if (cparam == "getsubdevices")
			{
				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;

				root["status"] = "OK";
				root["title"] = "GetSubDevices";
				result = m_sql.safe_query("SELECT a.ID, b.Name FROM LightSubDevices a, DeviceStatus b WHERE (a.ParentID=='%q') AND (b.ID == a.DeviceRowID)",
					idx.c_str());
				if (!result.empty())
				{
					int ii = 0;
					for (const auto & itt : result)
					{
						std::vector<std::string> sd = itt;

						root["result"][ii]["ID"] = sd[0];
						root["result"][ii]["Name"] = sd[1];
						ii++;
					}
				}
			}
			else if (cparam == "getscenedevices")
			{
				std::string idx = request::findValue(&req, "idx");
				std::string isscene = request::findValue(&req, "isscene");

				if (
					(idx.empty()) ||
					(isscene.empty())
					)
					return;

				root["status"] = "OK";
				root["title"] = "GetSceneDevices";

				result = m_sql.safe_query("SELECT a.ID, b.Name, a.DeviceRowID, b.Type, b.SubType, b.nValue, b.sValue, a.Cmd, a.Level, b.ID, a.[Order], a.Color, a.OnDelay, a.OffDelay, b.SwitchType FROM SceneDevices a, DeviceStatus b WHERE (a.SceneRowID=='%q') AND (b.ID == a.DeviceRowID) ORDER BY a.[Order]",
					idx.c_str());
				if (!result.empty())
				{
					int ii = 0;
					for (const auto & itt : result)
					{
						std::vector<std::string> sd = itt;

						root["result"][ii]["ID"] = sd[0];
						root["result"][ii]["Name"] = sd[1];
						root["result"][ii]["DevID"] = sd[2];
						root["result"][ii]["DevRealIdx"] = sd[9];
						root["result"][ii]["Order"] = atoi(sd[10].c_str());
						root["result"][ii]["OnDelay"] = atoi(sd[12].c_str());
						root["result"][ii]["OffDelay"] = atoi(sd[13].c_str());

						_eSwitchType switchtype = (_eSwitchType)atoi(sd[14].c_str());

						unsigned char devType = atoi(sd[3].c_str());

						if (devType != pTypeRFY)
							switchtype = STYPE_OnOff;

						unsigned char subType = atoi(sd[4].c_str());
						unsigned char nValue = (unsigned char)atoi(sd[5].c_str());
						std::string sValue = sd[6];
						int command = atoi(sd[7].c_str());
						int level = atoi(sd[8].c_str());

						std::string lstatus = "";
						int llevel = 0;
						bool bHaveDimmer = false;
						bool bHaveGroupCmd = false;
						int maxDimLevel = 0;
						GetLightStatus(devType, subType, switchtype, command, sValue, lstatus, llevel, bHaveDimmer, maxDimLevel, bHaveGroupCmd);
						root["result"][ii]["Command"] = lstatus;
						root["result"][ii]["Level"] = level;
						root["result"][ii]["Color"] = _tColor(sd[11]).toJSONString();
						root["result"][ii]["Type"] = RFX_Type_Desc(devType, 1);
						root["result"][ii]["SubType"] = RFX_Type_SubType_Desc(devType, subType);
						ii++;
					}
				}
			}
			else if (cparam == "changescenedeviceorder")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				std::string sway = request::findValue(&req, "way");
				if (sway.empty())
					return;
				bool bGoUp = (sway == "0");

				std::string aScene, aOrder, oID, oOrder;

				result = m_sql.safe_query("SELECT SceneRowID, [Order] FROM SceneDevices WHERE (ID=='%q')",
					idx.c_str());
				if (result.empty())
					return;
				aScene = result[0][0];
				aOrder = result[0][1];

				if (!bGoUp)
				{
					result = m_sql.safe_query("SELECT ID, [Order] FROM SceneDevices WHERE (SceneRowID=='%q' AND [Order]>'%q') ORDER BY [Order] ASC",
						aScene.c_str(), aOrder.c_str());
					if (result.empty())
						return;
					oID = result[0][0];
					oOrder = result[0][1];
				}
				else
				{
					result = m_sql.safe_query("SELECT ID, [Order] FROM SceneDevices WHERE (SceneRowID=='%q' AND [Order]<'%q') ORDER BY [Order] DESC",
						aScene.c_str(), aOrder.c_str());
					if (result.empty())
						return;
					oID = result[0][0];
					oOrder = result[0][1];
				}
				root["status"] = "OK";
				root["title"] = "ChangeSceneDeviceOrder";

				result = m_sql.safe_query("UPDATE SceneDevices SET [Order] = '%q' WHERE (ID='%q')",
					oOrder.c_str(), idx.c_str());
				result = m_sql.safe_query("UPDATE SceneDevices SET [Order] = '%q' WHERE (ID='%q')",
					aOrder.c_str(), oID.c_str());
			}
			else if (cparam == "deleteallscenedevices")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				root["status"] = "OK";
				root["title"] = "DeleteAllSceneDevices";
				result = m_sql.safe_query("DELETE FROM SceneDevices WHERE (SceneRowID == %q)", idx.c_str());
			}
			else if (cparam == "getmanualhardware")
			{
				root["status"] = "OK";
				root["title"] = "GetHardware";
				result = m_sql.safe_query("SELECT ID, Name, Type FROM Hardware ORDER BY ID ASC");
				if (!result.empty())
				{
					int ii = 0;
					for (const auto & itt : result)
					{
						std::vector<std::string> sd = itt;

						int ID = atoi(sd[0].c_str());
						std::string Name = sd[1];
						_eHardwareTypes Type = (_eHardwareTypes)atoi(sd[2].c_str());

						if (
							(Type == HTYPE_RFXLAN) ||
							(Type == HTYPE_RFXtrx315) ||
							(Type == HTYPE_RFXtrx433) ||
							(Type == HTYPE_RFXtrx868) ||
							(Type == HTYPE_EnOceanESP2) ||
							(Type == HTYPE_EnOceanESP3) ||
							(Type == HTYPE_Dummy) ||
							(Type == HTYPE_Tellstick) ||
							(Type == HTYPE_EVOHOME_SCRIPT) ||
							(Type == HTYPE_EVOHOME_SERIAL) ||
							(Type == HTYPE_EVOHOME_WEB) ||
							(Type == HTYPE_EVOHOME_TCP) ||
							(Type == HTYPE_RaspberryGPIO) ||
							(Type == HTYPE_RFLINKUSB) ||
							(Type == HTYPE_RFLINKTCP) ||
							(Type == HTYPE_ZIBLUEUSB) ||
							(Type == HTYPE_ZIBLUETCP) ||
							(Type == HTYPE_OpenWebNetTCP) ||
							(Type == HTYPE_OpenWebNetUSB) ||
							(Type == HTYPE_SysfsGpio) ||
							(Type == HTYPE_USBtinGateway)
							)
						{
							root["result"][ii]["idx"] = ID;
							root["result"][ii]["Name"] = Name;
							ii++;
						}
					}
				}
			}
			else if (cparam == "getgpio")
			{
				root["title"] = "GetGpio";
#ifdef WITH_GPIO
				std::vector<CGpioPin> pins = CGpio::GetPinList();
				if (pins.size() == 0) {
					root["status"] = "ERROR";
					root["result"][0]["idx"] = 0;
					root["result"][0]["Name"] = "GPIO INIT ERROR";
				}
				else {
					int ii = 0;
					for (const auto & it : pins)
					{
						CGpioPin pin = it;
						root["status"] = "OK";
						root["result"][ii]["idx"] = pin.GetPin();
						root["result"][ii]["Name"] = pin.ToString();
						ii++;
					}
				}
#else
				root["status"] = "OK";
				root["result"][0]["idx"] = 0;
				root["result"][0]["Name"] = "N/A";
#endif
			}
			else if (cparam == "getsysfsgpio")
			{
				root["title"] = "GetSysfsGpio";
#ifdef WITH_GPIO
				std::vector<int> gpio_ids = CSysfsGpio::GetGpioIds();
				std::vector<std::string> gpio_names = CSysfsGpio::GetGpioNames();

				if (gpio_ids.size() == 0) {
					root["status"] = "ERROR";
					root["result"][0]["idx"] = 0;
					root["result"][0]["Name"] = "No sysfs-gpio exports";
				}
				else {
					for (int ii = 0; ii < gpio_ids.size(); ii++)
					{
						root["status"] = "OK";
						root["result"][ii]["idx"] = gpio_ids[ii];
						root["result"][ii]["Name"] = gpio_names[ii];
			}
				}
#else
				root["status"] = "OK";
				root["result"][0]["idx"] = 0;
				root["result"][0]["Name"] = "N/A";
#endif
			}
			else if (cparam == "getlightswitches")
			{
				root["status"] = "OK";
				root["title"] = "GetLightSwitches";
				result = m_sql.safe_query("SELECT ID, Name, Type, SubType, Used, SwitchType, Options FROM DeviceStatus ORDER BY Name");
				if (!result.empty())
				{
					int ii = 0;
					for (const auto & itt : result)
					{
						std::vector<std::string> sd = itt;

						std::string ID = sd[0];
						std::string Name = sd[1];
						int Type = atoi(sd[2].c_str());
						int SubType = atoi(sd[3].c_str());
						int used = atoi(sd[4].c_str());
						_eSwitchType switchtype = (_eSwitchType)atoi(sd[5].c_str());
						std::map<std::string, std::string> options = m_sql.BuildDeviceOptions(sd[6]);
						bool bdoAdd = false;
						switch (Type)
						{
						case pTypeLighting1:
						case pTypeLighting2:
						case pTypeLighting3:
						case pTypeLighting4:
						case pTypeLighting5:
						case pTypeLighting6:
						case pTypeFan:
						case pTypeColorSwitch:
						case pTypeSecurity1:
						case pTypeSecurity2:
						case pTypeEvohome:
						case pTypeEvohomeRelay:
						case pTypeCurtain:
						case pTypeBlinds:
						case pTypeRFY:
						case pTypeChime:
						case pTypeThermostat2:
						case pTypeThermostat3:
						case pTypeThermostat4:
						case pTypeRemote:
						case pTypeRadiator1:
						case pTypeGeneralSwitch:
						case pTypeHomeConfort:
						case pTypeFS20:
							bdoAdd = true;
							if (!used)
							{
								bdoAdd = false;
								bool bIsSubDevice = false;
								std::vector<std::vector<std::string> > resultSD;
								resultSD = m_sql.safe_query("SELECT ID FROM LightSubDevices WHERE (DeviceRowID=='%q')",
									sd[0].c_str());
								if (resultSD.size() > 0)
									bdoAdd = true;
							}
							if ((Type == pTypeRadiator1) && (SubType != sTypeSmartwaresSwitchRadiator))
								bdoAdd = false;
							if (bdoAdd)
							{
								int idx = atoi(ID.c_str());
								if (!IsIdxForUser(&session, idx))
									continue;
								root["result"][ii]["idx"] = ID;
								root["result"][ii]["Name"] = Name;
								root["result"][ii]["Type"] = RFX_Type_Desc(Type, 1);
								root["result"][ii]["SubType"] = RFX_Type_SubType_Desc(Type, SubType);
								bool bIsDimmer = (
									(switchtype == STYPE_Dimmer) ||
									(switchtype == STYPE_BlindsPercentage) ||
									(switchtype == STYPE_BlindsPercentageInverted) ||
									(switchtype == STYPE_Selector)
									);
								root["result"][ii]["IsDimmer"] = bIsDimmer;

								std::string dimmerLevels = "none";

								if (bIsDimmer)
								{
									std::stringstream ss;

									if (switchtype == STYPE_Selector) {
										std::map<std::string, std::string> selectorStatuses;
										GetSelectorSwitchStatuses(options, selectorStatuses);
										bool levelOffHidden = (options["LevelOffHidden"] == "true");
										for (int i = 0; i < (int)selectorStatuses.size(); i++) {
											if (levelOffHidden && (i == 0)) {
												continue;
											}
											if ((levelOffHidden && (i > 1)) || (i > 0)) {
												ss << ",";
											}
											ss << i * 10;
										}
									}
									else
									{
										int nValue = 0;
										std::string sValue = "";
										std::string lstatus = "";
										int llevel = 0;
										bool bHaveDimmer = false;
										int maxDimLevel = 0;
										bool bHaveGroupCmd = false;

										GetLightStatus(Type, SubType, switchtype, nValue, sValue, lstatus, llevel, bHaveDimmer, maxDimLevel, bHaveGroupCmd);

										for (int i = 0; i <= maxDimLevel; i++)
										{
											if (i != 0)
											{
												ss << ",";
											}
											ss << (int)float((100.0f / float(maxDimLevel))*i);
										}
									}
									dimmerLevels = ss.str();
								}
								root["result"][ii]["DimmerLevels"] = dimmerLevels;
								ii++;
							}
							break;
						}
					}
				}
			}
			else if (cparam == "getlightswitchesscenes")
			{
				root["status"] = "OK";
				root["title"] = "GetLightSwitchesScenes";
				int ii = 0;

				result = m_sql.safe_query("SELECT ID, Name, Type, SubType, Used FROM DeviceStatus ORDER BY Name");
				if (!result.empty())
				{
					for (const auto & itt : result)
					{
						std::vector<std::string> sd = itt;

						std::string ID = sd[0];
						std::string Name = sd[1];
						int Type = atoi(sd[2].c_str());
						int SubType = atoi(sd[3].c_str());
						int used = atoi(sd[4].c_str());
						if (used)
						{
							switch (Type)
							{
							case pTypeLighting1:
							case pTypeLighting2:
							case pTypeLighting3:
							case pTypeLighting4:
							case pTypeLighting5:
							case pTypeLighting6:
							case pTypeFan:
							case pTypeColorSwitch:
							case pTypeSecurity1:
							case pTypeSecurity2:
							case pTypeEvohome:
							case pTypeEvohomeRelay:
							case pTypeCurtain:
							case pTypeBlinds:
							case pTypeRFY:
							case pTypeChime:
							case pTypeThermostat2:
							case pTypeThermostat3:
							case pTypeThermostat4:
							case pTypeRemote:
							case pTypeGeneralSwitch:
							case pTypeHomeConfort:
							case pTypeFS20:
								root["result"][ii]["type"] = 0;
								root["result"][ii]["idx"] = ID;
								root["result"][ii]["Name"] = "[Light/Switch] " + Name;
								ii++;
								break;
							case pTypeRadiator1:
								if (SubType == sTypeSmartwaresSwitchRadiator)
								{
									root["result"][ii]["type"] = 0;
									root["result"][ii]["idx"] = ID;
									root["result"][ii]["Name"] = "[Light/Switch] " + Name;
									ii++;
								}
								break;
							}
						}
					}
				}//end light/switches

				result = m_sql.safe_query("SELECT ID, Name FROM Scenes ORDER BY Name");
				if (!result.empty())
				{
					for (const auto & itt : result)
					{
						std::vector<std::string> sd = itt;

						std::string ID = sd[0];
						std::string Name = sd[1];

						root["result"][ii]["type"] = 1;
						root["result"][ii]["idx"] = ID;
						root["result"][ii]["Name"] = "[Scene] " + Name;
						ii++;
					}
				}//end light/switches
			}
			else if (cparam == "getcamactivedevices")
			{
				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				root["status"] = "OK";
				root["title"] = "GetCameraActiveDevices";
				result = m_sql.safe_query("SELECT ID, DevSceneType, DevSceneRowID, DevSceneWhen, DevSceneDelay FROM CamerasActiveDevices WHERE (CameraRowID=='%q') ORDER BY ID",
					idx.c_str());
				if (!result.empty())
				{
					int ii = 0;
					for (const auto & itt : result)
					{
						std::vector<std::string> sd = itt;

						std::string ID = sd[0];
						int DevSceneType = atoi(sd[1].c_str());
						std::string DevSceneRowID = sd[2];
						int DevSceneWhen = atoi(sd[3].c_str());
						int DevSceneDelay = atoi(sd[4].c_str());

						std::string Name = "";
						if (DevSceneType == 0)
						{
							std::vector<std::vector<std::string> > result2;
							result2 = m_sql.safe_query("SELECT Name FROM DeviceStatus WHERE (ID=='%q')",
								DevSceneRowID.c_str());
							if (!result2.empty())
							{
								Name = "[Light/Switches] " + result2[0][0];
							}
						}
						else
						{
							std::vector<std::vector<std::string> > result2;
							result2 = m_sql.safe_query("SELECT Name FROM Scenes WHERE (ID=='%q')",
								DevSceneRowID.c_str());
							if (!result2.empty())
							{
								Name = "[Scene] " + result2[0][0];
							}
						}
						if (Name != "")
						{
							root["result"][ii]["idx"] = ID;
							root["result"][ii]["type"] = DevSceneType;
							root["result"][ii]["DevSceneRowID"] = DevSceneRowID;
							root["result"][ii]["when"] = DevSceneWhen;
							root["result"][ii]["delay"] = DevSceneDelay;
							root["result"][ii]["Name"] = Name;
							ii++;
						}
					}
				}
			}
			else if (cparam == "addcamactivedevice")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				std::string activeidx = request::findValue(&req, "activeidx");
				std::string sactivetype = request::findValue(&req, "activetype");
				std::string sactivewhen = request::findValue(&req, "activewhen");
				std::string sactivedelay = request::findValue(&req, "activedelay");

				if (
					(idx.empty()) ||
					(activeidx.empty()) ||
					(sactivetype.empty()) ||
					(sactivewhen.empty()) ||
					(sactivedelay.empty())
					)
				{
					return;
				}

				int activetype = atoi(sactivetype.c_str());
				int activewhen = atoi(sactivewhen.c_str());
				int activedelay = atoi(sactivedelay.c_str());

				result = m_sql.safe_query(
					"SELECT ID FROM CamerasActiveDevices WHERE (CameraRowID=='%q')"
					" AND (DevSceneType==%d) AND (DevSceneRowID=='%q')"
					" AND (DevSceneWhen==%d)",
					idx.c_str(), activetype, activeidx.c_str(), activewhen);
				if (result.empty())
				{
					root["status"] = "OK";
					root["title"] = "AddCameraActiveDevice";
					result = m_sql.safe_query(
						"INSERT INTO CamerasActiveDevices (CameraRowID, DevSceneType, DevSceneRowID, DevSceneWhen, DevSceneDelay) VALUES ('%q',%d,'%q',%d,%d)",
						idx.c_str(),
						activetype,
						activeidx.c_str(),
						activewhen,
						activedelay
					);
					m_mainworker.m_cameras.ReloadCameras();
				}
			}
			else if (cparam == "deleteamactivedevice")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				root["status"] = "OK";
				root["title"] = "DeleteCameraActiveDevice";
				result = m_sql.safe_query("DELETE FROM CamerasActiveDevices WHERE (ID == '%q')", idx.c_str());
				m_mainworker.m_cameras.ReloadCameras();
			}
			else if (cparam == "deleteallactivecamdevices")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				root["status"] = "OK";
				root["title"] = "DeleteAllCameraActiveDevices";
				result = m_sql.safe_query("DELETE FROM CamerasActiveDevices WHERE (CameraRowID == '%q')", idx.c_str());
				m_mainworker.m_cameras.ReloadCameras();
			}
			else if (cparam == "testnotification")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string notification_Title = "Domoticz test";
				std::string notification_Message = "Domoticz test message!";
				std::string subsystem = request::findValue(&req, "subsystem");

				std::string extraData = request::findValue(&req, "extradata");

				m_notifications.ConfigFromGetvars(req, false);
				if (m_notifications.SendMessage(0, std::string(""), subsystem, notification_Title, notification_Message, extraData, 1, std::string(""), false)) {
					root["status"] = "OK";
				}
				/* we need to reload the config, because the values that were set were only for testing */
				m_notifications.LoadConfig();
			}
			else if (cparam == "testswitch")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string hwdid = request::findValue(&req, "hwdid");
				std::string sswitchtype = request::findValue(&req, "switchtype");
				std::string slighttype = request::findValue(&req, "lighttype");

				if (
					(hwdid.empty()) ||
					(sswitchtype.empty()) ||
					(slighttype.empty())
					)
					return;
				_eSwitchType switchtype = (_eSwitchType)atoi(sswitchtype.c_str());
				int lighttype = atoi(slighttype.c_str());
				int dtype;
				int subtype = 0;
				std::string sunitcode;
				std::string devid;

				if (lighttype == 70)
				{
					dtype = pTypeLighting2;
					subtype = sTypeAC;
					std::string sgroupcode = request::findValue(&req, "groupcode");
					sunitcode = request::findValue(&req, "unitcode");
					int iUnitTest = atoi(sunitcode.c_str());	//only First Rocker_ID at the moment, gives us 128 devices we can control, should be enough!
					if (
						(sunitcode.empty()) ||
						(sgroupcode.empty()) ||
						((iUnitTest < 1) || (iUnitTest > 128))
						)
						return;
					sunitcode = sgroupcode;//Button A or B
					CDomoticzHardwareBase *pBaseHardware = reinterpret_cast<CDomoticzHardwareBase*>(m_mainworker.GetHardware(atoi(hwdid.c_str())));
					if (pBaseHardware == NULL)
						return;
					if ((pBaseHardware->HwdType != HTYPE_EnOceanESP2) && (pBaseHardware->HwdType != HTYPE_EnOceanESP3)
						&& (pBaseHardware->HwdType != HTYPE_USBtinGateway) )
						return;
					unsigned long rID = 0;
					if (pBaseHardware->HwdType == HTYPE_EnOceanESP2)
					{
						CEnOceanESP2 *pEnoceanHardware = reinterpret_cast<CEnOceanESP2 *>(pBaseHardware);
						rID = pEnoceanHardware->m_id_base + iUnitTest;
					}
					else if (pBaseHardware->HwdType == HTYPE_EnOceanESP3)
					{
						CEnOceanESP3 *pEnoceanHardware = reinterpret_cast<CEnOceanESP3 *>(pBaseHardware);
						rID = pEnoceanHardware->m_id_base + iUnitTest;
					}
					else if (pBaseHardware->HwdType == HTYPE_USBtinGateway) //Like EnOcean (Lighting2 with Base_ID offset)
					{
						USBtin *pUSBtinHardware = reinterpret_cast<USBtin *>(pBaseHardware);
						rID = pUSBtinHardware->switch_id_base;
						std::stringstream ssunitcode;
						ssunitcode << iUnitTest;
						sunitcode = ssunitcode.str();
					}
					std::stringstream s_strid;
					s_strid << std::hex << std::uppercase << rID;
					devid = s_strid.str();
				}
				else if (lighttype == 68)
				{
#ifdef WITH_GPIO
					dtype = pTypeLighting1;
					subtype = sTypeIMPULS;
					devid = "0";
					sunitcode = request::findValue(&req, "unitcode"); //Unit code = GPIO number

					if (sunitcode.empty()) {
						root["status"] = "ERROR";
						root["message"] = "No GPIO number given";
						return;
					}
					CGpio *pGpio = reinterpret_cast<CGpio *>(m_mainworker.GetHardware(atoi(hwdid.c_str())));
					if (pGpio == NULL) {
						root["status"] = "ERROR";
						root["message"] = "Could not retrieve GPIO hardware pointer";
						return;
					}
					if (pGpio->HwdType != HTYPE_RaspberryGPIO) {
						root["status"] = "ERROR";
						root["message"] = "Given hardware is not GPIO";
						return;
					}
					CGpioPin *pPin = CGpio::GetPPinById(atoi(sunitcode.c_str()));
					if (pPin == NULL) {
						root["status"] = "ERROR";
						root["message"] = "Given pin does not exist on this GPIO hardware";
						return;
					}
					if (pPin->GetIsInput()) {
						root["status"] = "ERROR";
						root["message"] = "Given pin is not configured for output";
						return;
			}
#else
					root["status"] = "ERROR";
					root["message"] = "GPIO support is disabled";
					return;
#endif
				}
				else if (lighttype == 69)
				{
#ifdef WITH_GPIO

					sunitcode = request::findValue(&req, "unitcode"); // sysfs-gpio number
					int unitcode = atoi(sunitcode.c_str());
					dtype = pTypeLighting2;
					subtype = sTypeAC;
					std::string sswitchtype = request::findValue(&req, "switchtype");
					_eSwitchType switchtype = (_eSwitchType)atoi(sswitchtype.c_str());

					std::string id = request::findValue(&req, "id");
					if ((id.empty()) || (sunitcode.empty()))
					{
						return;
					}
					devid = id;

					if (sunitcode.empty())
					{
						root["status"] = "ERROR";
						root["message"] = "No GPIO number given";
						return;
					}

					CSysfsGpio *pSysfsGpio = reinterpret_cast<CSysfsGpio *>(m_mainworker.GetHardware(atoi(hwdid.c_str())));

					if (pSysfsGpio == NULL) {
						root["status"] = "ERROR";
						root["message"] = "Could not retrieve SysfsGpio hardware pointer";
						return;
					}

					if (pSysfsGpio->HwdType != HTYPE_SysfsGpio) {
						root["status"] = "ERROR";
						root["message"] = "Given hardware is not SysfsGpio";
						return;
					}
#else
					root["status"] = "ERROR";
					root["message"] = "GPIO support is disabled";
					return;
#endif
				}
				else if (lighttype < 20)
				{
					dtype = pTypeLighting1;
					subtype = lighttype;
					std::string shousecode = request::findValue(&req, "housecode");
					sunitcode = request::findValue(&req, "unitcode");
					if (
						(shousecode.empty()) ||
						(sunitcode.empty())
						)
						return;
					devid = shousecode;
				}
				else if (lighttype < 30)
				{
					dtype = pTypeLighting2;
					subtype = lighttype - 20;
					std::string id = request::findValue(&req, "id");
					sunitcode = request::findValue(&req, "unitcode");
					if (
						(id.empty()) ||
						(sunitcode.empty())
						)
						return;
					devid = id;
				}
				else if (lighttype < 70)
				{
					dtype = pTypeLighting5;
					subtype = lighttype - 50;
					if (lighttype == 59)
						subtype = sTypeIT;
					std::string id = request::findValue(&req, "id");
					sunitcode = request::findValue(&req, "unitcode");
					if (
						(id.empty()) ||
						(sunitcode.empty())
						)
						return;
					if ((subtype != sTypeEMW100) && (subtype != sTypeLivolo) && (subtype != sTypeLivolo1to10) && (subtype != sTypeRGB432W) && (subtype != sTypeIT))
						devid = "00" + id;
					else
						devid = id;
				}
				else
				{
					if (lighttype == 100)
					{
						dtype = pTypeChime;
						subtype = sTypeByronSX;
						std::string id = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(id.empty()) ||
							(sunitcode.empty())
							)
							return;
						int iUnitCode = atoi(sunitcode.c_str()) - 1;
						switch (iUnitCode)
						{
						case 0:
							iUnitCode = chime_sound0;
							break;
						case 1:
							iUnitCode = chime_sound1;
							break;
						case 2:
							iUnitCode = chime_sound2;
							break;
						case 3:
							iUnitCode = chime_sound3;
							break;
						case 4:
							iUnitCode = chime_sound4;
							break;
						case 5:
							iUnitCode = chime_sound5;
							break;
						case 6:
							iUnitCode = chime_sound6;
							break;
						case 7:
							iUnitCode = chime_sound7;
							break;
						}
						sprintf(szTmp, "%d", iUnitCode);
						sunitcode = szTmp;
						devid = id;
					}
					else if (lighttype == 101)
					{
						dtype = pTypeCurtain;
						subtype = sTypeHarrison;
						std::string shousecode = request::findValue(&req, "housecode");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(shousecode.empty()) ||
							(sunitcode.empty())
							)
							return;
						devid = shousecode;
					}
					else if (lighttype == 102)
					{
						dtype = pTypeRFY;
						subtype = sTypeRFY;
						std::string id = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(id.empty()) ||
							(sunitcode.empty())
							)
							return;
						devid = id;
					}
					else if (lighttype == 103)
					{
						dtype = pTypeSecurity1;
						subtype = sTypeMeiantech;
						std::string id = request::findValue(&req, "id");
						if (
							(id.empty())
							)
							return;
						devid = id;
						sunitcode = "0";
					}
					else if (lighttype == 104)
					{
						dtype = pTypeThermostat2;
						subtype = sTypeHE105;
						sunitcode = request::findValue(&req, "unitcode");
						if (sunitcode.empty())
							return;
						std::stringstream s_strid;
						s_strid << std::hex << std::uppercase << sunitcode;
						int iUnitCode;
						s_strid >> iUnitCode;
						sprintf(szTmp, "%d", iUnitCode);
						sunitcode = szTmp;
						devid = "1";
					}
					else if (lighttype == 105)
					{
						dtype = pTypeRFY;
						subtype = sTypeASA;
						std::string id = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(id.empty()) ||
							(sunitcode.empty())
							)
							return;
						devid = id;
					}
					else if (lighttype == 106)
					{
						dtype = pTypeLighting6;
						subtype = sTypeBlyss;
						std::string sgroupcode = request::findValue(&req, "groupcode");
						sunitcode = request::findValue(&req, "unitcode");
						std::string id = request::findValue(&req, "id");
						if (
							(sgroupcode.empty()) ||
							(sunitcode.empty()) ||
							(id.empty())
							)
							return;
						devid = id + sgroupcode;
					}
					else if (lighttype == 107)
					{
						dtype = pTypeRFY;
						subtype = sTypeRFY2;
						std::string id = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(id.empty()) ||
							(sunitcode.empty())
							)
							return;
						devid = id;
					}
					else if ((lighttype >= 200) && (lighttype < 300))
					{
						dtype = pTypeBlinds;
						subtype = lighttype - 200;
						std::string id = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(id.empty()) ||
							(sunitcode.empty())
							)
							return;
						int iUnitCode = atoi(sunitcode.c_str());
						sprintf(szTmp, "%d", iUnitCode);
						sunitcode = szTmp;
						devid = id;
					}
					else if (lighttype == 301)
					{
						dtype = pTypeRadiator1;
						subtype = sTypeSmartwaresSwitchRadiator;
						std::string id = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(id.empty()) ||
							(sunitcode.empty())
							)
							return;
						devid = id;
					}
					else if (lighttype == 302)
					{
						dtype = pTypeHomeConfort;
						subtype = sTypeHomeConfortTEL010;
						std::string id = request::findValue(&req, "id");

						std::string shousecode = request::findValue(&req, "housecode");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(id.empty()) ||
							(shousecode.empty()) ||
							(sunitcode.empty())
							)
							return;

						int iUnitCode = atoi(sunitcode.c_str());
						sprintf(szTmp, "%d", iUnitCode);
						sunitcode = szTmp;
						sprintf(szTmp, "%02X", atoi(shousecode.c_str()));
						shousecode = szTmp;
						devid = id + shousecode;
					}
					else if (lighttype == 303)
					{
						dtype = pTypeGeneralSwitch;
						subtype = sSwitchTypeSelector;
						std::string id = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(id.empty()) ||
							(sunitcode.empty())
							)
							return;
						devid = id;
					}
					else if (lighttype == 304)
					{
						dtype = pTypeFan;
						subtype = sTypeItho;
						std::string id = request::findValue(&req, "id");
						if (id.empty())
							return;
						devid = id;
						sunitcode = "0";
					}
					else if (lighttype == 305)
					{
						dtype = pTypeFan;
						subtype = sTypeLucciAir;
						std::string id = request::findValue(&req, "id");
						if (id.empty())
							return;
						devid = id;
						sunitcode = "0";
					}
					else if (lighttype == 306)
					{
						dtype = pTypeFan;
						subtype = sTypeLucciAirDC;
						std::string id = request::findValue(&req, "id");
						if (id.empty())
							return;
						devid = id;
						sunitcode = "0";
					}
					else if (lighttype == 307)
					{
						dtype = pTypeFan;
						subtype = sTypeWestinghouse;
						std::string id = request::findValue(&req, "id");
						if (id.empty())
							return;
						devid = id;
						sunitcode = "0";
					}
					else if (lighttype == 400) {
						dtype = pTypeGeneralSwitch;
						subtype = sSwitchBlindsT1;
						devid = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(devid.empty()) ||
							(sunitcode.empty())
							)
							return;
					}
					else if (lighttype == 401) {
						dtype = pTypeGeneralSwitch;
						subtype = sSwitchLightT1;
						devid = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(devid.empty()) ||
							(sunitcode.empty())
							)
							return;
					}
					else if (lighttype == 402)
					{
						dtype = pTypeGeneralSwitch;
						subtype = sSwitchAuxiliaryT1;
						devid = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(devid.empty()) ||
							(sunitcode.empty())
							)
							return;
					}
					else if (lighttype == 403) {
						dtype = pTypeGeneralSwitch;
						subtype = sSwitchBlindsT2;
						devid = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(devid.empty()) ||
							(sunitcode.empty())
							)
							return;
					}
					else if (lighttype == 404) {
						dtype = pTypeGeneralSwitch;
						subtype = sSwitchLightT2;
						devid = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(devid.empty()) ||
							(sunitcode.empty())
							)
							return;
					}
					else if ((lighttype == 405) || (lighttype == 406)) {
						dtype = pTypeGeneralSwitch;
						subtype = sSwitchContactT1;
						devid = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(devid.empty()) ||
							(sunitcode.empty())
							)
							return;
					}
				}
				CDomoticzHardwareBase *pBaseHardware = reinterpret_cast<CDomoticzHardwareBase*>(m_mainworker.GetHardware(atoi(hwdid.c_str())));
				if (pBaseHardware != NULL)
				{
					if ((pBaseHardware->HwdType == HTYPE_RFLINKUSB) || (pBaseHardware->HwdType == HTYPE_RFLINKTCP)) {
						ConvertToGeneralSwitchType(devid, dtype, subtype);
					}
				}

				root["status"] = "OK";
				root["message"] = "OK";
				root["title"] = "TestSwitch";
				std::vector<std::string> sd;

				sd.push_back(hwdid);
				sd.push_back(devid);
				sd.push_back(sunitcode);
				sprintf(szTmp, "%d", dtype);
				sd.push_back(szTmp);
				sprintf(szTmp, "%d", subtype);
				sd.push_back(szTmp);
				sprintf(szTmp, "%d", switchtype);
				sd.push_back(szTmp);
				sd.push_back(""); //AddjValue2
				sd.push_back(""); //nValue
				sd.push_back(""); //sValue
				sd.push_back(""); //Name
				sd.push_back(""); //Options

				std::string switchcmd = "On";
				int level = 0;
				if (lighttype == 70)
				{
					if (switchtype == STYPE_Dimmer)
						level = 5;
				}
				m_mainworker.SwitchLightInt(sd, switchcmd, level, NoColor, true);
			}
			else if (cparam == "addswitch")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string hwdid = request::findValue(&req, "hwdid");
				std::string name = request::findValue(&req, "name");
				std::string sswitchtype = request::findValue(&req, "switchtype");
				std::string slighttype = request::findValue(&req, "lighttype");
				std::string maindeviceidx = request::findValue(&req, "maindeviceidx");
				std::string deviceoptions;

				if (
					(hwdid.empty()) ||
					(sswitchtype.empty()) ||
					(slighttype.empty()) ||
					(name.empty())
					)
					return;
				_eSwitchType switchtype = (_eSwitchType)atoi(sswitchtype.c_str());
				int lighttype = atoi(slighttype.c_str());
				int dtype = 0;
				int subtype = 0;
				std::string sunitcode;
				std::string devid;

#ifdef ENABLE_PYTHON
				{
					result = m_sql.safe_query("SELECT Type FROM Hardware WHERE (ID == '%q')", hwdid.c_str());
					if (!result.empty())
					{
						std::vector<std::string> sd = result[0];
						_eHardwareTypes Type = (_eHardwareTypes)atoi(sd[0].c_str());
						if (Type == HTYPE_PythonPlugin)
						{
							_log.Log(LOG_ERROR, "CWebServer::HandleCommand addswitch: Not allowed to add device owned by plugin %u!", atoi(hwdid.c_str()));
							root["message"] = "Not allowed to add switch to plugin HW!";
							return;
						}
					}
				}
#endif

				if (lighttype == 70)
				{
					dtype = pTypeLighting2;
					subtype = sTypeAC;
					sunitcode = request::findValue(&req, "unitcode");
					std::string sgroupcode = request::findValue(&req, "groupcode");
					int iUnitTest = atoi(sunitcode.c_str());	//gives us 128 devices we can control, should be enough!
					if (
						(sunitcode.empty()) ||
						(sgroupcode.empty()) ||
						((iUnitTest < 1) || (iUnitTest > 128))
						)
						return;
					sunitcode = sgroupcode;//Button A/B
					CDomoticzHardwareBase *pBaseHardware = reinterpret_cast<CDomoticzHardwareBase*>(m_mainworker.GetHardware(atoi(hwdid.c_str())));
					if (pBaseHardware == NULL)
						return;
					if ((pBaseHardware->HwdType != HTYPE_EnOceanESP2) && (pBaseHardware->HwdType != HTYPE_EnOceanESP3)
						&& (pBaseHardware->HwdType != HTYPE_USBtinGateway) )
						return;
					unsigned long rID = 0;
					if (pBaseHardware->HwdType == HTYPE_EnOceanESP2)
					{
						CEnOceanESP2 *pEnoceanHardware = reinterpret_cast<CEnOceanESP2*>(pBaseHardware);
						if (pEnoceanHardware->m_id_base == 0)
						{
							root["message"] = "BaseID not found, is the hardware running?";
							return;
						}
						rID = pEnoceanHardware->m_id_base + iUnitTest;
					}
					else if (pBaseHardware->HwdType == HTYPE_EnOceanESP3)
					{
						CEnOceanESP3 *pEnoceanHardware = reinterpret_cast<CEnOceanESP3*>(pBaseHardware);
						if (pEnoceanHardware->m_id_base == 0)
						{
							root["message"] = "BaseID not found, is the hardware running?";
							return;
						}
						rID = pEnoceanHardware->m_id_base + iUnitTest;
					}
					else if (pBaseHardware->HwdType == HTYPE_USBtinGateway)
					{
						USBtin *pUSBtinHardware = reinterpret_cast<USBtin *>(pBaseHardware);
						rID = pUSBtinHardware->switch_id_base;
						std::stringstream ssunitcode;
						ssunitcode << iUnitTest;
						sunitcode = ssunitcode.str();
					}
					std::stringstream s_strid;
					s_strid << std::hex << std::uppercase << rID;
					devid = s_strid.str();
				}
				else if (lighttype == 68)
				{
#ifdef WITH_GPIO
					dtype = pTypeLighting1;
					subtype = sTypeIMPULS;
					devid = "0";
					sunitcode = request::findValue(&req, "unitcode"); //Unit code = GPIO number

					if (sunitcode.empty()) {
						return;
					}
					CGpio *pGpio = reinterpret_cast<CGpio *>(m_mainworker.GetHardware(atoi(hwdid.c_str())));
					if (pGpio == NULL) {
						return;
					}
					if (pGpio->HwdType != HTYPE_RaspberryGPIO) {
						return;
					}
					CGpioPin *pPin = CGpio::GetPPinById(atoi(sunitcode.c_str()));
					if (pPin == NULL) {
						return;
			}
#else
					return;
#endif
				}
				else if (lighttype == 69)
				{
#ifdef WITH_GPIO
					dtype = pTypeLighting2;
					subtype = sTypeAC;
					devid = "0";
					sunitcode = request::findValue(&req, "unitcode"); // sysfs-gpio number
					int unitcode = atoi(sunitcode.c_str());
					std::string sswitchtype = request::findValue(&req, "switchtype");
					_eSwitchType switchtype = (_eSwitchType)atoi(sswitchtype.c_str());
					std::string id = request::findValue(&req, "id");
					CSysfsGpio::RequestDbUpdate(unitcode);

					if ((id.empty()) || (sunitcode.empty()))
					{
						return;
					}
					devid = id;

					CSysfsGpio *pSysfsGpio = reinterpret_cast<CSysfsGpio *>(m_mainworker.GetHardware(atoi(hwdid.c_str())));

					if ((pSysfsGpio == NULL) || (pSysfsGpio->HwdType != HTYPE_SysfsGpio))
					{
						return;
					}
#else
					return;
#endif
				}
				else if (lighttype < 20)
				{
					dtype = pTypeLighting1;
					subtype = lighttype;
					std::string shousecode = request::findValue(&req, "housecode");
					sunitcode = request::findValue(&req, "unitcode");
					if (
						(shousecode.empty()) ||
						(sunitcode.empty())
						)
						return;
					devid = shousecode;
				}
				else if (lighttype < 30)
				{
					dtype = pTypeLighting2;
					subtype = lighttype - 20;
					std::string id = request::findValue(&req, "id");
					sunitcode = request::findValue(&req, "unitcode");
					if (
						(id.empty()) ||
						(sunitcode.empty())
						)
						return;
					devid = id;
				}
				else if (lighttype < 70)
				{
					dtype = pTypeLighting5;
					subtype = lighttype - 50;
					if (lighttype == 59)
						subtype = sTypeIT;
					std::string id = request::findValue(&req, "id");
					sunitcode = request::findValue(&req, "unitcode");
					if (
						(id.empty()) ||
						(sunitcode.empty())
						)
						return;
					if ((subtype != sTypeEMW100) && (subtype != sTypeLivolo) && (subtype != sTypeLivolo1to10) && (subtype != sTypeRGB432W) && (subtype != sTypeLightwaveRF) && (subtype != sTypeIT))
						devid = "00" + id;
					else
						devid = id;
				}
				else if (lighttype == 101)
				{
					dtype = pTypeCurtain;
					subtype = sTypeHarrison;
					std::string shousecode = request::findValue(&req, "housecode");
					sunitcode = request::findValue(&req, "unitcode");
					if (
						(shousecode.empty()) ||
						(sunitcode.empty())
						)
						return;
					devid = shousecode;
				}
				else if (lighttype == 102)
				{
					dtype = pTypeRFY;
					subtype = sTypeRFY;
					std::string id = request::findValue(&req, "id");
					sunitcode = request::findValue(&req, "unitcode");
					if (
						(id.empty()) ||
						(sunitcode.empty())
						)
						return;
					devid = id;
				}
				else if (lighttype == 103)
				{
					dtype = pTypeSecurity1;
					subtype = sTypeMeiantech;
					std::string id = request::findValue(&req, "id");
					if (
						(id.empty())
						)
						return;
					devid = id;
					sunitcode = "0";
				}
				else if (lighttype == 104)
				{
					dtype = pTypeThermostat2;
					subtype = sTypeHE105;
					sunitcode = request::findValue(&req, "unitcode");
					if (sunitcode.empty())
						return;
					std::stringstream s_strid;
					s_strid << std::hex << std::uppercase << sunitcode;
					int iUnitCode;
					s_strid >> iUnitCode;
					sprintf(szTmp, "%d", iUnitCode);
					sunitcode = szTmp;
					devid = "1";
				}
				else if (lighttype == 105)
				{
					dtype = pTypeRFY;
					subtype = sTypeASA;
					std::string id = request::findValue(&req, "id");
					sunitcode = request::findValue(&req, "unitcode");
					if (
						(id.empty()) ||
						(sunitcode.empty())
						)
						return;
					devid = id;
				}
				else if (lighttype == 106)
				{
					dtype = pTypeLighting6;
					subtype = sTypeBlyss;
					std::string sgroupcode = request::findValue(&req, "groupcode");
					sunitcode = request::findValue(&req, "unitcode");
					std::string id = request::findValue(&req, "id");
					if (
						(sgroupcode.empty()) ||
						(sunitcode.empty()) ||
						(id.empty())
						)
						return;
					devid = id + sgroupcode;
				}
				else if (lighttype == 107)
				{
					dtype = pTypeRFY;
					subtype = sTypeRFY2;
					std::string id = request::findValue(&req, "id");
					sunitcode = request::findValue(&req, "unitcode");
					if (
						(id.empty()) ||
						(sunitcode.empty())
						)
						return;
					devid = id;
				}
				else
				{
					if (lighttype == 100)
					{
						dtype = pTypeChime;
						subtype = sTypeByronSX;
						std::string id = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(id.empty()) ||
							(sunitcode.empty())
							)
							return;
						int iUnitCode = atoi(sunitcode.c_str()) - 1;
						switch (iUnitCode)
						{
						case 0:
							iUnitCode = chime_sound0;
							break;
						case 1:
							iUnitCode = chime_sound1;
							break;
						case 2:
							iUnitCode = chime_sound2;
							break;
						case 3:
							iUnitCode = chime_sound3;
							break;
						case 4:
							iUnitCode = chime_sound4;
							break;
						case 5:
							iUnitCode = chime_sound5;
							break;
						case 6:
							iUnitCode = chime_sound6;
							break;
						case 7:
							iUnitCode = chime_sound7;
							break;
						}
						sprintf(szTmp, "%d", iUnitCode);
						sunitcode = szTmp;
						devid = id;
					}
					else if ((lighttype >= 200) && (lighttype < 300))
					{
						dtype = pTypeBlinds;
						subtype = lighttype - 200;
						std::string id = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(id.empty()) ||
							(sunitcode.empty())
							)
							return;
						int iUnitCode = atoi(sunitcode.c_str());
						sprintf(szTmp, "%d", iUnitCode);
						sunitcode = szTmp;
						devid = id;
					}
					else if (lighttype == 301)
					{
						std::string id = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(id.empty()) ||
							(sunitcode.empty())
							)
							return;
						devid = id;

						dtype = pTypeRadiator1;
						subtype = sTypeSmartwares;

						result = m_sql.safe_query(
							"SELECT Name FROM DeviceStatus WHERE (HardwareID=='%q' AND DeviceID=='%q' AND Unit=='%q' AND Type==%d AND SubType==%d)",
							hwdid.c_str(), devid.c_str(), sunitcode.c_str(), dtype, subtype);
						if (!result.empty())
						{
							root["message"] = "Switch already exists!";
							return;
						}
						bool bActEnabledState = m_sql.m_bAcceptNewHardware;
						m_sql.m_bAcceptNewHardware = true;
						std::string devname;
						m_sql.UpdateValue(atoi(hwdid.c_str()), devid.c_str(), atoi(sunitcode.c_str()), dtype, subtype, 0, -1, 0, "20.5", devname);
						m_sql.m_bAcceptNewHardware = bActEnabledState;

						result = m_sql.safe_query(
							"SELECT ID FROM DeviceStatus WHERE (HardwareID=='%q' AND DeviceID=='%q' AND Unit=='%q' AND Type==%d AND SubType==%d)",
							hwdid.c_str(), devid.c_str(), sunitcode.c_str(), dtype, subtype);
						if (result.empty())
						{
							root["message"] = "Error finding switch in Database!?!?";
							return;
						}
						std::string ID = result[0][0];

						m_sql.safe_query(
							"UPDATE DeviceStatus SET Used=1, Name='%q', SwitchType=%d WHERE (ID == '%q')",
							name.c_str(), switchtype, ID.c_str());

						dtype = pTypeRadiator1;
						subtype = sTypeSmartwaresSwitchRadiator;
					}
					else if (lighttype == 302)
					{
						dtype = pTypeHomeConfort;
						subtype = sTypeHomeConfortTEL010;
						std::string id = request::findValue(&req, "id");

						std::string shousecode = request::findValue(&req, "housecode");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(id.empty()) ||
							(shousecode.empty()) ||
							(sunitcode.empty())
							)
							return;

						int iUnitCode = atoi(sunitcode.c_str());
						sprintf(szTmp, "%d", iUnitCode);
						sunitcode = szTmp;
						sprintf(szTmp, "%02X", atoi(shousecode.c_str()));
						shousecode = szTmp;
						devid = id + shousecode;
					}
					else if (lighttype == 303)
					{
						dtype = pTypeGeneralSwitch;
						subtype = sSwitchTypeSelector;
						std::string id = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(id.empty()) ||
							(sunitcode.empty())
							)
							return;
						devid = "0" + id;
						switchtype = STYPE_Selector;
						if (!deviceoptions.empty()) {
							deviceoptions.append(";");
						}
						deviceoptions.append("SelectorStyle:0;LevelNames:Off|Level1|Level2|Level3");
					}
					else if (lighttype == 304)
					{
						dtype = pTypeFan;
						subtype = sTypeItho;
						std::string id = request::findValue(&req, "id");
						if (id.empty())
							return;
						devid = id;
						sunitcode = "0";
					}
					else if (lighttype == 305)
					{
						dtype = pTypeFan;
						subtype = sTypeLucciAir;
						std::string id = request::findValue(&req, "id");
						if (id.empty())
							return;
						devid = id;
						sunitcode = "0";
					}
					else if (lighttype == 306)
					{
						dtype = pTypeFan;
						subtype = sTypeLucciAirDC;
						std::string id = request::findValue(&req, "id");
						if (id.empty())
							return;
						devid = id;
						sunitcode = "0";
					}
					else if (lighttype == 307)
					{
						dtype = pTypeFan;
						subtype = sTypeWestinghouse;
						std::string id = request::findValue(&req, "id");
						if (id.empty())
							return;
						devid = id;
						sunitcode = "0";
					}
					else if (lighttype == 400)
					{
						dtype = pTypeGeneralSwitch;
						subtype = sSwitchBlindsT1;
						devid = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(devid.empty()) ||
							(sunitcode.empty())
							)
							return;
					}
					else if (lighttype == 401)
					{
						dtype = pTypeGeneralSwitch;
						subtype = sSwitchLightT1;
						devid = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(devid.empty()) ||
							(sunitcode.empty())
							)
							return;
					}
					else if (lighttype == 402)
					{
						dtype = pTypeGeneralSwitch;
						subtype = sSwitchAuxiliaryT1;
						devid = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(devid.empty()) ||
							(sunitcode.empty())
							)
							return;
					}
					else if (lighttype == 403)
					{
						dtype = pTypeGeneralSwitch;
						subtype = sSwitchBlindsT2;
						devid = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(devid.empty()) ||
							(sunitcode.empty())
							)
							return;
					}
					else if (lighttype == 404)
					{
						dtype = pTypeGeneralSwitch;
						subtype = sSwitchLightT2;
						devid = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(devid.empty()) ||
							(sunitcode.empty())
							)
							return;
					}
					else if ((lighttype == 405) || (lighttype == 406))
					{
						dtype = pTypeGeneralSwitch;
						subtype = sSwitchContactT1;
						devid = request::findValue(&req, "id");
						sunitcode = request::findValue(&req, "unitcode");
						if (
							(devid.empty()) ||
							(sunitcode.empty())
							)
							return;
					}
				}

				result = m_sql.safe_query(
					"SELECT Name FROM DeviceStatus WHERE (HardwareID=='%q' AND DeviceID=='%q' AND Unit=='%q' AND Type==%d AND SubType==%d)",
					hwdid.c_str(), devid.c_str(), sunitcode.c_str(), dtype, subtype);
				if (!result.empty())
				{
					root["message"] = "Switch already exists!";
					return;
				}

				CDomoticzHardwareBase *pBaseHardware = m_mainworker.GetHardware(atoi(hwdid.c_str()));
				if (pBaseHardware != NULL)
				{
					if ((pBaseHardware->HwdType == HTYPE_RFLINKUSB) || (pBaseHardware->HwdType == HTYPE_RFLINKTCP)) {
						ConvertToGeneralSwitchType(devid, dtype, subtype);
					}
				}

				bool bActEnabledState = m_sql.m_bAcceptNewHardware;
				m_sql.m_bAcceptNewHardware = true;
				std::string devname;
				m_sql.UpdateValue(atoi(hwdid.c_str()), devid.c_str(), atoi(sunitcode.c_str()), dtype, subtype, 0, -1, 0, devname);
				m_sql.m_bAcceptNewHardware = bActEnabledState;

				result = m_sql.safe_query(
					"SELECT ID FROM DeviceStatus WHERE (HardwareID=='%q' AND DeviceID=='%q' AND Unit=='%q' AND Type==%d AND SubType==%d)",
					hwdid.c_str(), devid.c_str(), sunitcode.c_str(), dtype, subtype);
				if (result.empty())
				{
					root["message"] = "Error finding switch in Database!?!?";
					return;
				}
				std::string ID = result[0][0];

				m_sql.safe_query(
					"UPDATE DeviceStatus SET Used=1, Name='%q', SwitchType=%d WHERE (ID == '%q')",
					name.c_str(), switchtype, ID.c_str());
				m_mainworker.m_eventsystem.GetCurrentStates();

				m_sql.SetDeviceOptions(atoi(ID.c_str()), m_sql.BuildDeviceOptions(deviceoptions, false));

				if (maindeviceidx != "")
				{
					if (maindeviceidx != ID)
					{
						result = m_sql.safe_query(
							"SELECT ID FROM LightSubDevices WHERE (DeviceRowID=='%q') AND (ParentID =='%q')",
							ID.c_str(), maindeviceidx.c_str());
						if (result.empty())
						{
							result = m_sql.safe_query(
								"INSERT INTO LightSubDevices (DeviceRowID, ParentID) VALUES ('%q','%q')",
								ID.c_str(),
								maindeviceidx.c_str()
							);
						}
					}
				}

				root["status"] = "OK";
				root["title"] = "AddSwitch";
			}
			else if (cparam == "getnotificationtypes")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				result = m_sql.safe_query("SELECT Type, SubType, SwitchType FROM DeviceStatus WHERE (ID == '%q')",
					idx.c_str());
				if (result.empty())
					return;

				root["status"] = "OK";
				root["title"] = "GetNotificationTypes";
				unsigned char dType = atoi(result[0][0].c_str());
				unsigned char dSubType = atoi(result[0][1].c_str());
				unsigned char switchtype = atoi(result[0][2].c_str());

				int ii = 0;
				if (
					(dType == pTypeLighting1) ||
					(dType == pTypeLighting2) ||
					(dType == pTypeLighting3) ||
					(dType == pTypeLighting4) ||
					(dType == pTypeLighting5) ||
					(dType == pTypeLighting6) ||
					(dType == pTypeColorSwitch) ||
					(dType == pTypeSecurity1) ||
					(dType == pTypeSecurity2) ||
					(dType == pTypeEvohome) ||
					(dType == pTypeEvohomeRelay) ||
					(dType == pTypeCurtain) ||
					(dType == pTypeBlinds) ||
					(dType == pTypeRFY) ||
					(dType == pTypeChime) ||
					(dType == pTypeThermostat2) ||
					(dType == pTypeThermostat3) ||
					(dType == pTypeThermostat4) ||
					(dType == pTypeRemote) ||
					(dType == pTypeGeneralSwitch) ||
					(dType == pTypeHomeConfort) ||
					(dType == pTypeFS20) ||
					((dType == pTypeRadiator1) && (dSubType == sTypeSmartwaresSwitchRadiator))
					)
				{
					if (switchtype != STYPE_PushOff)
					{
						root["result"][ii]["val"] = NTYPE_SWITCH_ON;
						root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_SWITCH_ON, 0);
						root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_SWITCH_ON, 1);
						ii++;
					}
					if (switchtype != STYPE_PushOn)
					{
						root["result"][ii]["val"] = NTYPE_SWITCH_OFF;
						root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_SWITCH_OFF, 0);
						root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_SWITCH_OFF, 1);
						ii++;
					}
					if (switchtype == STYPE_Media)
					{
						std::string idx = request::findValue(&req, "idx");

						result = m_sql.safe_query("SELECT HardwareID FROM DeviceStatus WHERE (ID=='%q')", idx.c_str());
						if (!result.empty())
						{
							std::string hdwid = result[0][0];
							CDomoticzHardwareBase *pBaseHardware = reinterpret_cast<CDomoticzHardwareBase*>(m_mainworker.GetHardware(atoi(hdwid.c_str())));
							if (pBaseHardware != NULL) {
								_eHardwareTypes type = pBaseHardware->HwdType;
								root["result"][ii]["val"] = NTYPE_PAUSED;
								root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_PAUSED, 0);
								root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_PAUSED, 1);
								ii++;
								if (type == HTYPE_Kodi) {
									root["result"][ii]["val"] = NTYPE_AUDIO;
									root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_AUDIO, 0);
									root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_AUDIO, 1);
									ii++;
									root["result"][ii]["val"] = NTYPE_VIDEO;
									root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_VIDEO, 0);
									root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_VIDEO, 1);
									ii++;
									root["result"][ii]["val"] = NTYPE_PHOTO;
									root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_PHOTO, 0);
									root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_PHOTO, 1);
									ii++;
								}
								if (type == HTYPE_LogitechMediaServer) {
									root["result"][ii]["val"] = NTYPE_PLAYING;
									root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_PLAYING, 0);
									root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_PLAYING, 1);
									ii++;
									root["result"][ii]["val"] = NTYPE_STOPPED;
									root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_STOPPED, 0);
									root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_STOPPED, 1);
									ii++;
								}
								if (type == HTYPE_HEOS) {
									root["result"][ii]["val"] = NTYPE_PLAYING;
									root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_PLAYING, 0);
									root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_PLAYING, 1);
									ii++;
									root["result"][ii]["val"] = NTYPE_STOPPED;
									root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_STOPPED, 0);
									root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_STOPPED, 1);
									ii++;
								}
							}
						}
					}
				}
				if (
					(
					(dType == pTypeTEMP) ||
						(dType == pTypeTEMP_HUM) ||
						(dType == pTypeTEMP_HUM_BARO) ||
						(dType == pTypeTEMP_BARO) ||
						(dType == pTypeEvohomeZone) ||
						(dType == pTypeEvohomeWater) ||
						(dType == pTypeThermostat1) ||
						(dType == pTypeRego6XXTemp) ||
						((dType == pTypeRFXSensor) && (dSubType == sTypeRFXSensorTemp))
						) ||
						((dType == pTypeUV) && (dSubType == sTypeUV3)) ||
					((dType == pTypeWIND) && (dSubType == sTypeWIND4)) ||
					((dType == pTypeWIND) && (dSubType == sTypeWINDNoTemp)) ||
					((dType == pTypeGeneral) && (dSubType == sTypeSystemTemp))
					)
				{
					root["result"][ii]["val"] = NTYPE_TEMPERATURE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_TEMPERATURE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_TEMPERATURE, 1);
					ii++;
				}
				if (
					(dType == pTypeHUM) ||
					(dType == pTypeTEMP_HUM) ||
					(dType == pTypeTEMP_HUM_BARO)
					)
				{
					root["result"][ii]["val"] = NTYPE_HUMIDITY;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_HUMIDITY, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_HUMIDITY, 1);
					ii++;
				}
				if (
					(dType == pTypeTEMP_HUM) ||
					(dType == pTypeTEMP_HUM_BARO)
					)
				{
					root["result"][ii]["val"] = NTYPE_DEWPOINT;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_DEWPOINT, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_DEWPOINT, 1);
					ii++;
				}
				if (dType == pTypeRAIN)
				{
					root["result"][ii]["val"] = NTYPE_RAIN;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_RAIN, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_RAIN, 1);
					ii++;
				}
				if (dType == pTypeWIND)
				{
					root["result"][ii]["val"] = NTYPE_WIND;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_WIND, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_WIND, 1);
					ii++;
				}
				if (dType == pTypeUV)
				{
					root["result"][ii]["val"] = NTYPE_UV;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_UV, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_UV, 1);
					ii++;
				}
				if (
					(dType == pTypeTEMP_HUM_BARO) ||
					(dType == pTypeBARO) ||
					(dType == pTypeTEMP_BARO)
					)
				{
					root["result"][ii]["val"] = NTYPE_BARO;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_BARO, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_BARO, 1);
					ii++;
				}
				if (
					((dType == pTypeRFXMeter) && (dSubType == sTypeRFXMeterCount)) ||
					((dType == pTypeGeneral) && (dSubType == sTypeCounterIncremental)) ||
					(dType == pTypeYouLess) ||
					((dType == pTypeRego6XXValue) && (dSubType == sTypeRego6XXCounter))
					)
				{
					if ((switchtype == MTYPE_ENERGY) || (switchtype == MTYPE_ENERGY_GENERATED))
					{
						root["result"][ii]["val"] = NTYPE_TODAYENERGY;
						root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_TODAYENERGY, 0);
						root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_TODAYENERGY, 1);
					}
					else if (switchtype == MTYPE_GAS)
					{
						root["result"][ii]["val"] = NTYPE_TODAYGAS;
						root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_TODAYGAS, 0);
						root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_TODAYGAS, 1);
					}
					else if (switchtype == MTYPE_COUNTER)
					{
						root["result"][ii]["val"] = NTYPE_TODAYCOUNTER;
						root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_TODAYCOUNTER, 0);
						root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_TODAYCOUNTER, 1);
					}
					else
					{
						root["result"][ii]["val"] = NTYPE_TODAYGAS;
						root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_TODAYGAS, 0);
						root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_TODAYGAS, 1);
					}
					ii++;
				}
				if (dType == pTypeYouLess)
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if (dType == pTypeAirQuality)
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				else if ((dType == pTypeGeneral) && ((dSubType == sTypeSoilMoisture) || (dSubType == sTypeLeafWetness)))
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if ((dType == pTypeGeneral) && (dSubType == sTypeVisibility))
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if ((dType == pTypeGeneral) && (dSubType == sTypeDistance))
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if ((dType == pTypeGeneral) && (dSubType == sTypeSolarRadiation))
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if ((dType == pTypeGeneral) && (dSubType == sTypeVoltage))
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if ((dType == pTypeGeneral) && (dSubType == sTypeCurrent))
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if ((dType == pTypeGeneral) && (dSubType == sTypePressure))
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if ((dType == pTypeGeneral) && (dSubType == sTypeBaro))
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if (dType == pTypeLux)
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if ((dType == pTypeGeneral) && (dSubType == sTypeSoundLevel))
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if (dType == pTypeWEIGHT)
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if (dType == pTypeUsage)
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if (
					(dType == pTypeENERGY) ||
					((dType == pTypeGeneral) && (dSubType == sTypeKwh))
					)
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if (dType == pTypePOWER)
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if ((dType == pTypeCURRENT) && (dSubType == sTypeELEC1))
				{
					root["result"][ii]["val"] = NTYPE_AMPERE1;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_AMPERE1, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_AMPERE1, 1);
					ii++;
					root["result"][ii]["val"] = NTYPE_AMPERE2;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_AMPERE2, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_AMPERE2, 1);
					ii++;
					root["result"][ii]["val"] = NTYPE_AMPERE3;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_AMPERE3, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_AMPERE3, 1);
					ii++;
				}
				if ((dType == pTypeCURRENTENERGY) && (dSubType == sTypeELEC4))
				{
					root["result"][ii]["val"] = NTYPE_AMPERE1;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_AMPERE1, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_AMPERE1, 1);
					ii++;
					root["result"][ii]["val"] = NTYPE_AMPERE2;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_AMPERE2, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_AMPERE2, 1);
					ii++;
					root["result"][ii]["val"] = NTYPE_AMPERE3;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_AMPERE3, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_AMPERE3, 1);
					ii++;
				}
				if (dType == pTypeP1Power)
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
					root["result"][ii]["val"] = NTYPE_TODAYENERGY;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_TODAYENERGY, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_TODAYENERGY, 1);
					ii++;
				}
				if (dType == pTypeP1Gas)
				{
					root["result"][ii]["val"] = NTYPE_TODAYGAS;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_TODAYGAS, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_TODAYGAS, 1);
					ii++;
				}
				if ((dType == pTypeThermostat) && (dSubType == sTypeThermSetpoint))
				{
					root["result"][ii]["val"] = NTYPE_TEMPERATURE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_TEMPERATURE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_TEMPERATURE, 1);
					ii++;
				}
				if (dType == pTypeEvohomeZone)
				{
					root["result"][ii]["val"] = NTYPE_TEMPERATURE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_SETPOINT, 0); //FIXME NTYPE_SETPOINT implementation?
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_SETPOINT, 1);
					ii++;
				}
				if ((dType == pTypeRFXSensor) && ((dSubType == sTypeRFXSensorAD) || (dSubType == sTypeRFXSensorVolt)))
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if ((dType == pTypeGeneral) && (dSubType == sTypePercentage))
				{
					root["result"][ii]["val"] = NTYPE_PERCENTAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_PERCENTAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_PERCENTAGE, 1);
					ii++;
				}
				if ((dType == pTypeGeneral) && (dSubType == sTypeWaterflow))
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if ((dType == pTypeGeneral) && (dSubType == sTypeCustom))
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if ((dType == pTypeGeneral) && (dSubType == sTypeFan))
				{
					root["result"][ii]["val"] = NTYPE_RPM;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_RPM, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_RPM, 1);
					ii++;
				}
				if ((dType == pTypeGeneral) && (dSubType == sTypeAlert))
				{
					root["result"][ii]["val"] = NTYPE_USAGE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_USAGE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_USAGE, 1);
					ii++;
				}
				if ((dType == pTypeGeneral) && (dSubType == sTypeZWaveAlarm))
				{
					root["result"][ii]["val"] = NTYPE_VALUE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_VALUE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_VALUE, 1);
					ii++;
				}
				if ((dType == pTypeRego6XXValue) && (dSubType == sTypeRego6XXStatus))
				{
					root["result"][ii]["val"] = NTYPE_SWITCH_ON;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_SWITCH_ON, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_SWITCH_ON, 1);
					ii++;
					root["result"][ii]["val"] = NTYPE_SWITCH_OFF;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_SWITCH_OFF, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_SWITCH_OFF, 1);
					ii++;
				}
				if (!IsLightOrSwitch(dType, dSubType))
				{
					root["result"][ii]["val"] = NTYPE_LASTUPDATE;
					root["result"][ii]["text"] = Notification_Type_Desc(NTYPE_LASTUPDATE, 0);
					root["result"][ii]["ptag"] = Notification_Type_Desc(NTYPE_LASTUPDATE, 1);
					ii++;
				}
			}
			else if (cparam == "addnotification")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;

				std::string stype = request::findValue(&req, "ttype");
				std::string swhen = request::findValue(&req, "twhen");
				std::string svalue = request::findValue(&req, "tvalue");
				std::string scustommessage = request::findValue(&req, "tmsg");
				std::string sactivesystems = request::findValue(&req, "tsystems");
				std::string spriority = request::findValue(&req, "tpriority");
				std::string ssendalways = request::findValue(&req, "tsendalways");
				std::string srecovery = (request::findValue(&req, "trecovery") == "true") ? "1" : "0";

				if ((stype.empty()) || (swhen.empty()) || (svalue.empty()) || (spriority.empty()) || (ssendalways.empty()) || (srecovery.empty()))
					return;

				_eNotificationTypes ntype = (_eNotificationTypes)atoi(stype.c_str());
				std::string ttype = Notification_Type_Desc(ntype, 1);
				if (
					(ntype == NTYPE_SWITCH_ON) ||
					(ntype == NTYPE_SWITCH_OFF) ||
					(ntype == NTYPE_DEWPOINT)
					)
				{
					if ((ntype == NTYPE_SWITCH_ON) && (swhen == "2")) { // '='
						unsigned char twhen = '=';
						sprintf(szTmp, "%s;%c;%s", ttype.c_str(), twhen, svalue.c_str());
					}
					else
						strcpy(szTmp, ttype.c_str());
				}
				else
				{
					std::string twhen;
					if (swhen == "0")
						twhen = ">";
					else if (swhen == "1")
						twhen = ">=";
					else if (swhen == "2")
						twhen = "=";
					else if (swhen == "3")
						twhen = "!=";
					else if (swhen == "4")
						twhen = "<=";
					else
						twhen = "<";
					sprintf(szTmp, "%s;%s;%s;%s", ttype.c_str(), twhen.c_str(), svalue.c_str(), srecovery.c_str());
				}
				int priority = atoi(spriority.c_str());
				bool bOK = m_notifications.AddNotification(idx, szTmp, scustommessage, sactivesystems, priority, (ssendalways == "true") ? true : false);
				if (bOK) {
					root["status"] = "OK";
					root["title"] = "AddNotification";
				}
			}
			else if (cparam == "updatenotification")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				std::string devidx = request::findValue(&req, "devidx");
				if ((idx.empty()) || (devidx.empty()))
					return;

				std::string stype = request::findValue(&req, "ttype");
				std::string swhen = request::findValue(&req, "twhen");
				std::string svalue = request::findValue(&req, "tvalue");
				std::string scustommessage = request::findValue(&req, "tmsg");
				std::string sactivesystems = request::findValue(&req, "tsystems");
				std::string spriority = request::findValue(&req, "tpriority");
				std::string ssendalways = request::findValue(&req, "tsendalways");
				std::string srecovery = (request::findValue(&req, "trecovery") == "true") ? "1" : "0";

				if ((stype.empty()) || (swhen.empty()) || (svalue.empty()) || (spriority.empty()) || (ssendalways.empty()) || srecovery.empty())
					return;
				root["status"] = "OK";
				root["title"] = "UpdateNotification";

				std::string recoverymsg;
				if ((srecovery == "1") && (m_notifications.CustomRecoveryMessage(strtoull(idx.c_str(), NULL, 0), recoverymsg, true)))
				{
					scustommessage.append(";;");
					scustommessage.append(recoverymsg);
				}
				m_notifications.RemoveNotification(idx);

				_eNotificationTypes ntype = (_eNotificationTypes)atoi(stype.c_str());
				std::string ttype = Notification_Type_Desc(ntype, 1);
				if (
					(ntype == NTYPE_SWITCH_ON) ||
					(ntype == NTYPE_SWITCH_OFF) ||
					(ntype == NTYPE_DEWPOINT)
					)
				{
					if ((ntype == NTYPE_SWITCH_ON) && (swhen == "2")) { // '='
						unsigned char twhen = '=';
						sprintf(szTmp, "%s;%c;%s", ttype.c_str(), twhen, svalue.c_str());
					}
					else
						strcpy(szTmp, ttype.c_str());
				}
				else
				{
					std::string twhen;
					if (swhen == "0")
						twhen = ">";
					else if (swhen == "1")
						twhen = ">=";
					else if (swhen == "2")
						twhen = "=";
					else if (swhen == "3")
						twhen = "!=";
					else if (swhen == "4")
						twhen = "<=";
					else
						twhen = "<";
					sprintf(szTmp, "%s;%s;%s;%s", ttype.c_str(), twhen.c_str(), svalue.c_str(), srecovery.c_str());
				}
				int priority = atoi(spriority.c_str());
				m_notifications.AddNotification(devidx, szTmp, scustommessage, sactivesystems, priority, (ssendalways == "true") ? true : false);
			}
			else if (cparam == "deletenotification")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;

				root["status"] = "OK";
				root["title"] = "DeleteNotification";

				m_notifications.RemoveNotification(idx);
			}
			else if (cparam == "switchdeviceorder")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx1 = request::findValue(&req, "idx1");
				std::string idx2 = request::findValue(&req, "idx2");
				if ((idx1.empty()) || (idx2.empty()))
					return;
				std::string sroomid = request::findValue(&req, "roomid");
				int roomid = atoi(sroomid.c_str());

				std::string Order1, Order2;
				if (roomid == 0)
				{
					result = m_sql.safe_query("SELECT [Order] FROM DeviceStatus WHERE (ID == '%q')",
						idx1.c_str());
					if (result.empty())
						return;
					Order1 = result[0][0];

					result = m_sql.safe_query("SELECT [Order] FROM DeviceStatus WHERE (ID == '%q')",
						idx2.c_str());
					if (result.empty())
						return;
					Order2 = result[0][0];

					root["status"] = "OK";
					root["title"] = "SwitchDeviceOrder";

					if (atoi(Order1.c_str()) < atoi(Order2.c_str()))
					{
						m_sql.safe_query(
							"UPDATE DeviceStatus SET [Order] = [Order]+1 WHERE ([Order] >= '%q' AND [Order] < '%q')",
							Order1.c_str(), Order2.c_str());
					}
					else
					{
						m_sql.safe_query(
							"UPDATE DeviceStatus SET [Order] = [Order]-1 WHERE ([Order] > '%q' AND [Order] <= '%q')",
							Order2.c_str(), Order1.c_str());
					}

					m_sql.safe_query("UPDATE DeviceStatus SET [Order] = '%q' WHERE (ID == '%q')",
						Order1.c_str(), idx2.c_str());
				}
				else
				{
					result = m_sql.safe_query("SELECT [Order] FROM DeviceToPlansMap WHERE (DeviceRowID == '%q') AND (PlanID==%d)",
						idx1.c_str(), roomid);
					if (result.empty())
						return;
					Order1 = result[0][0];

					result = m_sql.safe_query("SELECT [Order] FROM DeviceToPlansMap WHERE (DeviceRowID == '%q') AND (PlanID==%d)",
						idx2.c_str(), roomid);
					if (result.empty())
						return;
					Order2 = result[0][0];

					root["status"] = "OK";
					root["title"] = "SwitchDeviceOrder";

					if (atoi(Order1.c_str()) < atoi(Order2.c_str()))
					{
						m_sql.safe_query(
							"UPDATE DeviceToPlansMap SET [Order] = [Order]+1 WHERE ([Order] >= '%q' AND [Order] < '%q') AND (PlanID==%d)",
							Order1.c_str(), Order2.c_str(), roomid);
					}
					else
					{
						m_sql.safe_query(
							"UPDATE DeviceToPlansMap SET [Order] = [Order]-1 WHERE ([Order] > '%q' AND [Order] <= '%q') AND (PlanID==%d)",
							Order2.c_str(), Order1.c_str(), roomid);
					}

					m_sql.safe_query("UPDATE DeviceToPlansMap SET [Order] = '%q' WHERE (DeviceRowID == '%q') AND (PlanID==%d)",
						Order1.c_str(), idx2.c_str(), roomid);
				}
			}
			else if (cparam == "switchsceneorder")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx1 = request::findValue(&req, "idx1");
				std::string idx2 = request::findValue(&req, "idx2");
				if ((idx1.empty()) || (idx2.empty()))
					return;

				std::string Order1, Order2;
				result = m_sql.safe_query("SELECT [Order] FROM Scenes WHERE (ID == '%q')",
					idx1.c_str());
				if (result.empty())
					return;
				Order1 = result[0][0];

				result = m_sql.safe_query("SELECT [Order] FROM Scenes WHERE (ID == '%q')",
					idx2.c_str());
				if (result.empty())
					return;
				Order2 = result[0][0];

				root["status"] = "OK";
				root["title"] = "SwitchSceneOrder";

				if (atoi(Order1.c_str()) < atoi(Order2.c_str()))
				{
					m_sql.safe_query(
						"UPDATE Scenes SET [Order] = [Order]+1 WHERE ([Order] >= '%q' AND [Order] < '%q')",
						Order1.c_str(), Order2.c_str());
				}
				else
				{
					m_sql.safe_query(
						"UPDATE Scenes SET [Order] = [Order]-1 WHERE ([Order] > '%q' AND [Order] <= '%q')",
						Order2.c_str(), Order1.c_str());
				}

				m_sql.safe_query("UPDATE Scenes SET [Order] = '%q' WHERE (ID == '%q')",
					Order1.c_str(), idx2.c_str());
			}
			else if (cparam == "clearnotifications")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;

				root["status"] = "OK";
				root["title"] = "ClearNotification";

				m_notifications.RemoveDeviceNotifications(idx);
			}
			else if (cparam == "adduser")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string senabled = request::findValue(&req, "enabled");
				std::string username = request::findValue(&req, "username");
				std::string password = request::findValue(&req, "password");
				std::string srights = request::findValue(&req, "rights");
				std::string sRemoteSharing = request::findValue(&req, "RemoteSharing");
				std::string sTabsEnabled = request::findValue(&req, "TabsEnabled");
				if (
					(senabled.empty()) ||
					(username.empty()) ||
					(password.empty()) ||
					(srights.empty()) ||
					(sRemoteSharing.empty()) ||
					(sTabsEnabled.empty())
					)
					return;
				int rights = atoi(srights.c_str());
				if (rights != 2)
				{
					if (!FindAdminUser())
					{
						root["message"] = "Add a Admin user first! (Or enable Settings/Website Protection)";
						return;
					}
				}
				root["status"] = "OK";
				root["title"] = "AddUser";
				m_sql.safe_query(
					"INSERT INTO Users (Active, Username, Password, Rights, RemoteSharing, TabsEnabled) VALUES (%d,'%q','%q','%d','%d','%d')",
					(senabled == "true") ? 1 : 0,
					base64_encode(username).c_str(),
					password.c_str(),
					rights,
					(sRemoteSharing == "true") ? 1 : 0,
					atoi(sTabsEnabled.c_str())
				);
				LoadUsers();
			}
			else if (cparam == "updateuser")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				std::string senabled = request::findValue(&req, "enabled");
				std::string username = request::findValue(&req, "username");
				std::string password = request::findValue(&req, "password");
				std::string srights = request::findValue(&req, "rights");
				std::string sRemoteSharing = request::findValue(&req, "RemoteSharing");
				std::string sTabsEnabled = request::findValue(&req, "TabsEnabled");
				if (
					(senabled.empty()) ||
					(username.empty()) ||
					(password.empty()) ||
					(srights.empty()) ||
					(sRemoteSharing.empty()) ||
					(sTabsEnabled.empty())
					)
					return;
				int rights = atoi(srights.c_str());
				if (rights != 2)
				{
					if (!FindAdminUser())
					{
						root["message"] = "Add a Admin user first! (Or enable Settings/Website Protection)";
						return;
					}
				}
				std::string sHashedUsername = base64_encode(username);

				std::string sOldUsername;
				std::string sOldPassword;
				result = m_sql.safe_query("SELECT Username, Password FROM Users WHERE (ID == '%q')", idx.c_str());
				if (result.size() == 1)
				{
					sOldUsername = result[0][0];
					sOldPassword = result[0][1];
				}
				if ((sHashedUsername != sOldUsername) || (password != sOldPassword))
					RemoveUsersSessions(sOldUsername, session);

				root["status"] = "OK";
				root["title"] = "UpdateUser";
				m_sql.safe_query(
					"UPDATE Users SET Active=%d, Username='%q', Password='%q', Rights=%d, RemoteSharing=%d, TabsEnabled=%d WHERE (ID == '%q')",
					(senabled == "true") ? 1 : 0,
					sHashedUsername.c_str(),
					password.c_str(),
					rights,
					(sRemoteSharing == "true") ? 1 : 0,
					atoi(sTabsEnabled.c_str()),
					idx.c_str()
				);
				LoadUsers();


			}
			else if (cparam == "deleteuser")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;

				root["status"] = "OK";
				root["title"] = "DeleteUser";

				result = m_sql.safe_query("SELECT Username FROM Users WHERE (ID == '%q')", idx.c_str());
				if (result.size() == 1)
				{
					RemoveUsersSessions(result[0][0], session);
				}

				m_sql.safe_query("DELETE FROM Users WHERE (ID == '%q')", idx.c_str());

				m_sql.safe_query("DELETE FROM SharedDevices WHERE (SharedUserID == '%q')", idx.c_str());

				LoadUsers();
			}
			else if (cparam == "clearlightlog")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				result = m_sql.safe_query("SELECT Type, SubType FROM DeviceStatus WHERE (ID == '%q')",
					idx.c_str());
				if (result.empty())
					return;

				unsigned char dType = atoi(result[0][0].c_str());
				unsigned char dSubType = atoi(result[0][1].c_str());

				if (
					(dType != pTypeLighting1) &&
					(dType != pTypeLighting2) &&
					(dType != pTypeLighting3) &&
					(dType != pTypeLighting4) &&
					(dType != pTypeLighting5) &&
					(dType != pTypeLighting6) &&
					(dType != pTypeFan) &&
					(dType != pTypeColorSwitch) &&
					(dType != pTypeSecurity1) &&
					(dType != pTypeSecurity2) &&
					(dType != pTypeEvohome) &&
					(dType != pTypeEvohomeRelay) &&
					(dType != pTypeCurtain) &&
					(dType != pTypeBlinds) &&
					(dType != pTypeRFY) &&
					(dType != pTypeChime) &&
					(dType != pTypeThermostat2) &&
					(dType != pTypeThermostat3) &&
					(dType != pTypeThermostat4) &&
					(dType != pTypeRemote) &&
					(dType != pTypeGeneralSwitch) &&
					(dType != pTypeHomeConfort) &&
					(dType != pTypeFS20) &&
					(!((dType == pTypeRadiator1) && (dSubType == sTypeSmartwaresSwitchRadiator))) &&
					(!((dType == pTypeGeneral) && (dSubType == sTypeTextStatus))) &&
					(!((dType == pTypeGeneral) && (dSubType == sTypeAlert)))
					)
					return; //no light device! we should not be here!

				root["status"] = "OK";
				root["title"] = "ClearLightLog";

				result = m_sql.safe_query("DELETE FROM LightingLog WHERE (DeviceRowID=='%q')", idx.c_str());
			}
			else if (cparam == "clearscenelog")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				root["status"] = "OK";
				root["title"] = "ClearSceneLog";

				result = m_sql.safe_query("DELETE FROM SceneLog WHERE (SceneRowID=='%q')", idx.c_str());
			}
			else if (cparam == "learnsw")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				m_sql.AllowNewHardwareTimer(5);
				m_sql.m_LastSwitchID = "";
				bool bReceivedSwitch = false;
				unsigned char cntr = 0;
				while ((!bReceivedSwitch) && (cntr < 50))	//wait for max. 5 seconds
				{
					if (m_sql.m_LastSwitchID != "")
					{
						bReceivedSwitch = true;
						break;
					}
					else
					{
						sleep_milliseconds(100);
						cntr++;
					}
				}
				if (bReceivedSwitch)
				{
					result = m_sql.safe_query("SELECT Name, Used, nValue FROM DeviceStatus WHERE (ID==%" PRIu64 ")",
						m_sql.m_LastSwitchRowID);
					if (!result.empty())
					{
						root["status"] = "OK";
						root["title"] = "LearnSW";
						root["ID"] = m_sql.m_LastSwitchID;
						root["idx"] = m_sql.m_LastSwitchRowID;
						root["Name"] = result[0][0];
						root["Used"] = atoi(result[0][1].c_str());
						root["Cmd"] = atoi(result[0][2].c_str());
					}
				}
			} //learnsw
			else if (cparam == "makefavorite")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				std::string sisfavorite = request::findValue(&req, "isfavorite");
				if ((idx.empty()) || (sisfavorite.empty()))
					return;
				int isfavorite = atoi(sisfavorite.c_str());
				m_sql.safe_query("UPDATE DeviceStatus SET Favorite=%d WHERE (ID == '%q')",
					isfavorite, idx.c_str());
				root["status"] = "OK";
				root["title"] = "MakeFavorite";
			} //makefavorite
			else if (cparam == "makescenefavorite")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				std::string sisfavorite = request::findValue(&req, "isfavorite");
				if ((idx.empty()) || (sisfavorite.empty()))
					return;
				int isfavorite = atoi(sisfavorite.c_str());
				m_sql.safe_query("UPDATE Scenes SET Favorite=%d WHERE (ID == '%q')",
					isfavorite, idx.c_str());
				root["status"] = "OK";
				root["title"] = "MakeSceneFavorite";
			} //makescenefavorite
			else if (cparam == "resetsecuritystatus")
			{
				std::string idx = request::findValue(&req, "idx");
				std::string switchcmd = request::findValue(&req, "switchcmd");

				if ((idx.empty()) || (switchcmd.empty()))
					return;

				root["status"] = "OK";
				root["title"] = "ResetSecurityStatus";

				int nValue = -1;


				if (switchcmd == "Panic End") {
					nValue = 7;
				}
				else if (switchcmd == "Normal") {
					nValue = 0;
				}

				if (nValue >= 0)
				{
					m_sql.safe_query("UPDATE DeviceStatus SET nValue=%d WHERE (ID == '%q')",
						nValue, idx.c_str());
					root["status"] = "OK";
					root["title"] = "SwitchLight";
				}
			}
			else if (cparam == "verifypasscode")
			{
				std::string passcode = request::findValue(&req, "passcode");
				if (passcode.empty())
					return;
				passcode = GenerateMD5Hash(passcode);
				std::string rpassword;
				int nValue = 1;
				m_sql.GetPreferencesVar("ProtectionPassword", nValue, rpassword);
				if (passcode == rpassword)
				{
					root["title"] = "VerifyPasscode";
					root["status"] = "OK";
					return;
				}
			}
			else if (cparam == "switchmodal")
			{
				int urights = 3;
				if (bHaveUser)
				{
					int iUser = -1;
					iUser = FindUser(session.username.c_str());
					if (iUser != -1)
					{
						urights = (int)m_users[iUser].userrights;
						_log.Log(LOG_STATUS, "User: %s initiated a modal command", m_users[iUser].Username.c_str());
					}
				}
				if (urights < 1)
					return;

				std::string idx = request::findValue(&req, "idx");
				std::string switchcmd = request::findValue(&req, "status");
				std::string until = request::findValue(&req, "until");//optional until date / time as applicable
				std::string action = request::findValue(&req, "action");//Run action or not (update status only)
				std::string onlyonchange = request::findValue(&req, "ooc");//No update unless the value changed (check if updated)
				if ((idx.empty()) || (switchcmd.empty()))
					return;

				std::string passcode = request::findValue(&req, "passcode");
				if (passcode.size() > 0)
				{
					passcode = GenerateMD5Hash(passcode);
					std::string rpassword;
					int nValue = 1;
					m_sql.GetPreferencesVar("ProtectionPassword", nValue, rpassword);
					if (passcode != rpassword)
					{
						root["title"] = "Modal";
						root["status"] = "ERROR";
						root["message"] = "WRONG CODE";
						return;
					}
				}

				if (m_mainworker.SwitchModal(idx, switchcmd, action, onlyonchange, until) == true)//FIXME we need to return a status of already set / no update if ooc=="1" and no status update was performed
				{
					root["status"] = "OK";
					root["title"] = "Modal";
				}
			}
			else if (cparam == "switchlight")
			{
				if (session.rights < 1)
				{
					session.reply_status = reply::forbidden;
					return; //Only user/admin allowed
				}
				std::string Username = "Admin";
				if (!session.username.empty())
					Username = session.username;

				std::string idx = request::findValue(&req, "idx");

				std::string switchcmd = request::findValue(&req, "switchcmd");
				std::string level = "-1";
				if (switchcmd == "Set Level")
					level = request::findValue(&req, "level");
				std::string onlyonchange = request::findValue(&req, "ooc");//No update unless the value changed (check if updated)
				_log.Debug(DEBUG_WEBSERVER, "WEBS switchlight idx:%s switchcmd:%s level:%s", idx.c_str(), switchcmd.c_str(), level.c_str());
				std::string passcode = request::findValue(&req, "passcode");
				if ((idx.empty()) || (switchcmd.empty()) || ((switchcmd == "Set Level") && (level.empty())) )
					return;

				result = m_sql.safe_query(
					"SELECT [Protected],[Name] FROM DeviceStatus WHERE (ID = '%q')", idx.c_str());
				if (result.empty())
				{
					return;
				}
				bool bIsProtected = atoi(result[0][0].c_str()) != 0;
				std::string sSwitchName = result[0][1];
				if (session.rights == 1)
				{
					if (!IsIdxForUser(&session, atoi(idx.c_str())))
					{
						_log.Log(LOG_ERROR, "User: %s initiated a Unauthorized switch command!", Username.c_str());
						session.reply_status = reply::forbidden;
						return;
					}
				}

				if (bIsProtected)
				{
					if (passcode.empty())
					{
						root["title"] = "SwitchLight";
						root["status"] = "ERROR";
						root["message"] = "WRONG CODE";
						return;
					}
					passcode = GenerateMD5Hash(passcode);
					std::string rpassword;
					int nValue = 1;
					m_sql.GetPreferencesVar("ProtectionPassword", nValue, rpassword);
					if (passcode != rpassword)
					{
						_log.Log(LOG_ERROR, "User: %s initiated a switch command (Wrong code!)", Username.c_str());
						root["title"] = "SwitchLight";
						root["status"] = "ERROR";
						root["message"] = "WRONG CODE";
						return;
					}
				}

				_log.Log(LOG_STATUS, "User: %s initiated a switch command (%s/%s/%s)", Username.c_str(), idx.c_str(), sSwitchName.c_str(), switchcmd.c_str());

				root["title"] = "SwitchLight";
				if (m_mainworker.SwitchLight(idx, switchcmd, level, "-1", onlyonchange, 0) == true)
				{
					root["status"] = "OK";
				}
				else
				{
					root["status"] = "ERROR";
					root["message"] = "Error sending switch command, check device/hardware !";
				}
			} //(rtype=="switchlight")
			else if (cparam == "switchscene")
			{
				if (session.rights < 1)
				{
					session.reply_status = reply::forbidden;
					return; //Only user/admin allowed
				}
				std::string Username = "Admin";
				if (!session.username.empty())
					Username = session.username;

				std::string idx = request::findValue(&req, "idx");
				std::string switchcmd = request::findValue(&req, "switchcmd");
				std::string passcode = request::findValue(&req, "passcode");
				if ((idx.empty()) || (switchcmd.empty()))
					return;

				result = m_sql.safe_query(
					"SELECT [Protected] FROM Scenes WHERE (ID = '%q')", idx.c_str());
				if (result.empty())
				{
					return;
				}
				bool bIsProtected = atoi(result[0][0].c_str()) != 0;
				if (bIsProtected)
				{
					if (passcode.empty())
					{
						root["title"] = "SwitchScene";
						root["status"] = "ERROR";
						root["message"] = "WRONG CODE";
						return;
					}
					passcode = GenerateMD5Hash(passcode);
					std::string rpassword;
					int nValue = 1;
					m_sql.GetPreferencesVar("ProtectionPassword", nValue, rpassword);
					if (passcode != rpassword)
					{
						root["title"] = "SwitchScene";
						root["status"] = "ERROR";
						root["message"] = "WRONG CODE";
						_log.Log(LOG_ERROR, "User: %s initiated a scene/group command (Wrong code!)", Username.c_str());
						return;
					}
				}
				_log.Log(LOG_STATUS, "User: %s initiated a scene/group command", Username.c_str());

				if (m_mainworker.SwitchScene(idx, switchcmd) == true)
				{
					root["status"] = "OK";
					root["title"] = "SwitchScene";
				}
			} //(rtype=="switchscene")
			else if (cparam == "getSunRiseSet") {
				if (!m_mainworker.m_LastSunriseSet.empty())
				{
					std::vector<std::string> strarray;
					StringSplit(m_mainworker.m_LastSunriseSet, ";", strarray);
					if (strarray.size() == 10)
					{
						struct tm loctime;
						time_t now = mytime(NULL);

						localtime_r(&now, &loctime);
						strftime(szTmp, 80, "%Y-%m-%d %X", &loctime);

						root["status"] = "OK";
						root["title"] = "getSunRiseSet";
						root["ServerTime"] = szTmp;
						root["Sunrise"] = strarray[0];
						root["Sunset"] = strarray[1];
						root["SunAtSouth"] = strarray[2];
						root["CivTwilightStart"] = strarray[3];
						root["CivTwilightEnd"] = strarray[4];
						root["NautTwilightStart"] = strarray[5];
						root["NautTwilightEnd"] = strarray[6];
						root["AstrTwilightStart"] = strarray[7];
						root["AstrTwilightEnd"] = strarray[8];
						root["DayLength"] = strarray[9];
					}
				}
			}
			else if (cparam == "getServerTime") {

				struct tm loctime;
				time_t now = mytime(NULL);

				localtime_r(&now, &loctime);
				strftime(szTmp, 80, "%Y-%m-%d %X", &loctime);

				root["status"] = "OK";
				root["title"] = "getServerTime";
				root["ServerTime"] = szTmp;
			}
			else if (cparam == "getsecstatus")
			{
				root["status"] = "OK";
				root["title"] = "GetSecStatus";

				int secstatus = 0;
				m_sql.GetPreferencesVar("SecStatus", secstatus);
				root["secstatus"] = secstatus;

				int secondelay = 30;
				m_sql.GetPreferencesVar("SecOnDelay", secondelay);
				root["secondelay"] = secondelay;
			}
			else if (cparam == "setsecstatus")
			{
				std::string ssecstatus = request::findValue(&req, "secstatus");
				std::string seccode = request::findValue(&req, "seccode");
				if ((ssecstatus.empty()) || (seccode.empty()))
				{
					root["message"] = "WRONG CODE";
					return;
				}
				root["title"] = "SetSecStatus";
				std::string rpassword;
				int nValue = 1;
				m_sql.GetPreferencesVar("SecPassword", nValue, rpassword);
				if (seccode != rpassword)
				{
					root["status"] = "ERROR";
					root["message"] = "WRONG CODE";
					return;
				}
				root["status"] = "OK";
				int iSecStatus = atoi(ssecstatus.c_str());
				m_mainworker.UpdateDomoticzSecurityStatus(iSecStatus);
			}
			else if (cparam == "setcolbrightnessvalue")
			{
				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}
				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				_tColor color;

				std::string json = request::findValue(&req, "color");
				std::string hex = request::findValue(&req, "hex");
				std::string hue = request::findValue(&req, "hue");
				std::string sat = request::findValue(&req, "sat");
				std::string brightness = request::findValue(&req, "brightness");
				std::string iswhite = request::findValue(&req, "iswhite");

				int ival = 100;
				float brightnessAdj = 1.0f;

				if (!json.empty())
				{
					color = _tColor(json);
					if (color.mode == ColorModeRGB)
					{
						float hsb[3];
						int r, g, b;
						rgb2hsb(color.r, color.g, color.b, hsb);
						hsb2rgb(hsb[0]*360.0f, hsb[1], 1.0f, r, g, b, 255);
						color.r = r;
						color.g = g;
						color.b = b;
						brightnessAdj = hsb[2];
					}

				}
				else if (!hex.empty())
				{
					uint64_t ihex = hexstrtoui64(hex);
					uint8_t r = 0;
					uint8_t g = 0;
					uint8_t b = 0;
					uint8_t cw = 0;
					uint8_t ww = 0;
					switch (hex.length())
					{
						case 6: //RGB
							r = (uint8_t)((ihex & 0x0000FF0000) >> 16);
							g = (uint8_t)((ihex & 0x000000FF00) >> 8);
							b = (uint8_t)ihex & 0xFF;
							float hsb[3];
							int tr, tg, tb; // tmp of 'int' type so can be passed as references to hsb2rgb
							rgb2hsb(r, g, b, hsb);
							hsb2rgb(hsb[0]*360.0f, hsb[1], 1.0f, tr, tg, tb, 255);
							r = tr;
							g = tg;
							b = tb;
							brightnessAdj = hsb[2];
							iswhite = (hsb[1] < (20.0 / 255.0)) ? "true" : "false";
							color = _tColor(r, g, b, cw, ww, ColorModeRGB);
							break;
						case 8: //RGB_WW
							r = (uint8_t)((ihex & 0x00FF000000) >> 24);
							g = (uint8_t)((ihex & 0x0000FF0000) >> 16);
							b = (uint8_t)((ihex & 0x000000FF00) >> 8);
							ww = (uint8_t)ihex & 0xFF;
							color = _tColor(r, g, b, cw, ww, ColorModeCustom);
							break;
						case 10: //RGB_CW_WW
							r = (uint8_t)((ihex & 0xFF00000000) >> 32);
							g = (uint8_t)((ihex & 0x00FF000000) >> 24);
							b = (uint8_t)((ihex & 0x0000FF0000) >> 16);
							cw = (uint8_t)((ihex & 0x000000FF00) >> 8);
							ww = (uint8_t)ihex & 0xFF;
							color = _tColor(r, g, b, cw, ww, ColorModeCustom);
							break;
					}
					if (iswhite == "true") color.mode = ColorModeWhite;
				}
				else if (!hue.empty())
				{
					int r, g, b;

					float iHue = float(atof(hue.c_str()));
					float iSat = 100.0f;
					if (!sat.empty()) iSat = float(atof(sat.c_str()));
					hsb2rgb(iHue, iSat/100.0f, 1.0f, r, g, b, 255);

					color = _tColor(r, g, b, 0, 0, ColorModeRGB);
					if (iswhite == "true") color.mode = ColorModeWhite;
				}

				if (color.mode == ColorModeNone)
				{
					return;
				}

				if (!brightness.empty())
					ival = atoi(brightness.c_str());
				ival = int(ival * brightnessAdj);
				ival = std::max(ival, 0);
				ival = std::min(ival, 100);

				_log.Log(LOG_STATUS, "setcolbrightnessvalue: ID: %" PRIx64 ", bri: %d, color: '%s'", ID, ival, color.toString().c_str());
				m_mainworker.SwitchLight(ID, "Set Color", (unsigned char)ival, color, false, 0);

				root["status"] = "OK";
				root["title"] = "SetColBrightnessValue";
			}
			else if (cparam.find("setkelvinlevel") == 0)
			{
				root["status"] = "OK";
				root["title"] = "Set Kelvin Level";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);

				std::string kelvin = request::findValue(&req, "kelvin");
				double ival = atof(kelvin.c_str());
				ival = std::max(ival, 0.0);
				ival = std::min(ival, 100.0);
				_tColor color = _tColor(round(ival*255.0f/100.0f), ColorModeTemp);
				_log.Log(LOG_STATUS, "setkelvinlevel: t: %f, color: '%s'", ival, color.toString().c_str());

				m_mainworker.SwitchLight(ID, "Set Color", -1, color, false, 0);
			}
			else if (cparam == "brightnessup")
			{
				root["status"] = "OK";
				root["title"] = "Set brightness up!";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				m_mainworker.SwitchLight(ID, "Bright Up", 0, NoColor, false, 0);
			}
			else if (cparam == "brightnessdown")
			{
				root["status"] = "OK";
				root["title"] = "Set brightness down!";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				m_mainworker.SwitchLight(ID, "Bright Down", 0, NoColor, false, 0);
			}
			else if (cparam == "discomode")
			{
				root["status"] = "OK";
				root["title"] = "Set to last known disco mode!";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				m_mainworker.SwitchLight(ID, "Disco Mode", 0, NoColor, false, 0);
			}
			else if (cparam.find("discomodenum") == 0 && cparam != "discomode" && cparam.size() == 13)
			{
				root["status"] = "OK";
				root["title"] = "Set to disco mode!";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				char szTmp[40];
				sprintf(szTmp, "Disco Mode %s", cparam.substr(12).c_str());
				m_mainworker.SwitchLight(ID, szTmp, 0, NoColor, false, 0);
			}
			else if (cparam == "discoup")
			{
				root["status"] = "OK";
				root["title"] = "Set to next disco mode!";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				m_mainworker.SwitchLight(ID, "Disco Up", 0, NoColor, false, 0);
			}
			else if (cparam == "discodown")
			{
				root["status"] = "OK";
				root["title"] = "Set to previous disco mode!";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				m_mainworker.SwitchLight(ID, "Disco Down", 0, NoColor, false, 0);
			}
			else if (cparam == "speedup")
			{
				root["status"] = "OK";
				root["title"] = "Set disco speed up!";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				m_mainworker.SwitchLight(ID, "Speed Up", 0, NoColor, false, 0);
			}
			else if (cparam == "speeduplong")
			{

				root["status"] = "OK";
				root["title"] = "Set speed long!";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				m_mainworker.SwitchLight(ID, "Speed Up Long", 0, NoColor, false, 0);
			}
			else if (cparam == "speeddown")
			{
				root["status"] = "OK";
				root["title"] = "Set disco speed down!";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				m_mainworker.SwitchLight(ID, "Speed Down", 0, NoColor, false, 0);
			}
			else if (cparam == "speedmin")
			{
				root["status"] = "OK";
				root["title"] = "Set disco speed minimal!";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				m_mainworker.SwitchLight(ID, "Speed Minimal", 0, NoColor, false, 0);
			}
			else if (cparam == "speedmax")
			{
				root["status"] = "OK";
				root["title"] = "Set disco speed maximal!";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				m_mainworker.SwitchLight(ID, "Speed Maximal", 0, NoColor, false, 0);
			}
			else if (cparam == "warmer")
			{
				root["status"] = "OK";
				root["title"] = "Set Kelvin up!";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				m_mainworker.SwitchLight(ID, "Warmer", 0, NoColor, false, 0);
			}
			else if (cparam == "cooler")
			{
				root["status"] = "OK";
				root["title"] = "Set Kelvin down!";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				m_mainworker.SwitchLight(ID, "Cooler", 0, NoColor, false, 0);
			}
			else if (cparam == "fulllight")
			{
				root["status"] = "OK";
				root["title"] = "Set Full!";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				m_mainworker.SwitchLight(ID, "Set Full", 0, NoColor, false, 0);
			}
			else if (cparam == "nightlight")
			{
				root["status"] = "OK";
				root["title"] = "Set to nightlight!";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				m_mainworker.SwitchLight(ID, "Set Night", 0, NoColor, false, 0);
			}
			else if (cparam == "whitelight")
			{
				root["status"] = "OK";
				root["title"] = "Set to clear white!";

				std::string idx = request::findValue(&req, "idx");

				if (idx.empty())
				{
					return;
				}

				uint64_t ID = std::strtoull(idx.c_str(), nullptr, 10);
				m_mainworker.SwitchLight(ID, "Set White", 0, NoColor, false, 0);
			}
			else if (cparam == "getfloorplanimages")
			{
				root["status"] = "OK";
				root["title"] = "GetFloorplanImages";

				bool bReturnUnused = atoi(request::findValue(&req, "unused").c_str()) != 0;

				if (!bReturnUnused)
					result = m_sql.safe_query("SELECT ID, Name, ScaleFactor FROM Floorplans ORDER BY [Name]");
				else
					result = m_sql.safe_query("SELECT ID, Name, ScaleFactor FROM Floorplans WHERE ID NOT IN(SELECT FloorplanID FROM Plans)");
				if (!result.empty())
				{
					int ii = 0;
					for (const auto & itt : result)
					{
						std::vector<std::string> sd = itt;

						root["result"][ii]["idx"] = sd[0];
						root["result"][ii]["name"] = sd[1];
						root["result"][ii]["scalefactor"] = sd[2];
						ii++;
					}
				}
			}
			else if (cparam == "updatefloorplan")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				std::string name = request::findValue(&req, "name");
				std::string scalefactor = request::findValue(&req, "scalefactor");
				if (
					(name.empty())
					||(scalefactor.empty())
					)
					return;

				root["status"] = "OK";
				root["title"] = "UpdateFloorplan";

				m_sql.safe_query(
					"UPDATE Floorplans SET Name='%q',ScaleFactor='%q' WHERE (ID == '%q')",
					name.c_str(),
					scalefactor.c_str(),
					idx.c_str()
				);
			}
			else if (cparam == "deletefloorplan")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				root["status"] = "OK";
				root["title"] = "DeleteFloorplan";
				m_sql.safe_query("UPDATE DeviceToPlansMap SET XOffset=0,YOffset=0 WHERE (PlanID IN (SELECT ID from Plans WHERE (FloorplanID == '%q')))", idx.c_str());
				m_sql.safe_query("UPDATE Plans SET FloorplanID=0,Area='' WHERE (FloorplanID == '%q')", idx.c_str());
				m_sql.safe_query("DELETE FROM Floorplans WHERE (ID == '%q')", idx.c_str());
			}
			else if (cparam == "changefloorplanorder")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				std::string sway = request::findValue(&req, "way");
				if (sway.empty())
					return;
				bool bGoUp = (sway == "0");

				std::string aOrder, oID, oOrder;

				result = m_sql.safe_query("SELECT [Order] FROM Floorplans WHERE (ID=='%q')",
					idx.c_str());
				if (result.empty())
					return;
				aOrder = result[0][0];

				if (!bGoUp)
				{
					result = m_sql.safe_query("SELECT ID, [Order] FROM Floorplans WHERE ([Order]>'%q') ORDER BY [Order] ASC",
						aOrder.c_str());
					if (result.empty())
						return;
					oID = result[0][0];
					oOrder = result[0][1];
				}
				else
				{
					result = m_sql.safe_query("SELECT ID, [Order] FROM Floorplans WHERE ([Order]<'%q') ORDER BY [Order] DESC",
						aOrder.c_str());
					if (result.empty())
						return;
					oID = result[0][0];
					oOrder = result[0][1];
				}
				root["status"] = "OK";
				root["title"] = "ChangeFloorPlanOrder";

				m_sql.safe_query("UPDATE Floorplans SET [Order] = '%q' WHERE (ID='%q')",
					oOrder.c_str(), idx.c_str());
				m_sql.safe_query("UPDATE Floorplans SET [Order] = '%q' WHERE (ID='%q')",
					aOrder.c_str(), oID.c_str());
			}
			else if (cparam == "getunusedfloorplanplans")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				root["status"] = "OK";
				root["title"] = "GetUnusedFloorplanPlans";
				int ii = 0;

				result = m_sql.safe_query("SELECT ID, Name FROM Plans WHERE (FloorplanID==0) ORDER BY Name");
				if (!result.empty())
				{
					for (const auto & itt : result)
					{
						std::vector<std::string> sd = itt;

						root["result"][ii]["type"] = 0;
						root["result"][ii]["idx"] = sd[0];
						root["result"][ii]["Name"] = sd[1];
						ii++;
					}
				}
			}
			else if (cparam == "getfloorplanplans")
			{
				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				root["status"] = "OK";
				root["title"] = "GetFloorplanPlans";
				int ii = 0;
				result = m_sql.safe_query("SELECT ID, Name, Area FROM Plans WHERE (FloorplanID=='%q') ORDER BY Name",
					idx.c_str());
				if (!result.empty())
				{
					for (const auto & itt : result)
					{
						std::vector<std::string> sd = itt;

						root["result"][ii]["idx"] = sd[0];
						root["result"][ii]["Name"] = sd[1];
						root["result"][ii]["Area"] = sd[2];
						ii++;
					}
				}
			}
			else if (cparam == "addfloorplanplan")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				std::string planidx = request::findValue(&req, "planidx");
				if (
					(idx.empty()) ||
					(planidx.empty())
					)
					return;
				root["status"] = "OK";
				root["title"] = "AddFloorplanPlan";

				m_sql.safe_query(
					"UPDATE Plans SET FloorplanID='%q' WHERE (ID == '%q')",
					idx.c_str(),
					planidx.c_str()
				);
				_log.Log(LOG_STATUS, "(Floorplan) Plan '%s' added to floorplan '%s'.", planidx.c_str(), idx.c_str());
			}
			else if (cparam == "updatefloorplanplan")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string planidx = request::findValue(&req, "planidx");
				std::string planarea = request::findValue(&req, "area");
				if (planidx.empty())
					return;
				root["status"] = "OK";
				root["title"] = "UpdateFloorplanPlan";

				m_sql.safe_query(
					"UPDATE Plans SET Area='%q' WHERE (ID == '%q')",
					planarea.c_str(),
					planidx.c_str()
				);
				_log.Log(LOG_STATUS, "(Floorplan) Plan '%s' floor area updated to '%s'.", planidx.c_str(), planarea.c_str());
			}
			else if (cparam == "deletefloorplanplan")
			{
				if (session.rights < 2)
				{
					session.reply_status = reply::forbidden;
					return; //Only admin user allowed
				}

				std::string idx = request::findValue(&req, "idx");
				if (idx.empty())
					return;
				root["status"] = "OK";
				root["title"] = "DeleteFloorplanPlan";
				m_sql.safe_query(
					"UPDATE DeviceToPlansMap SET XOffset=0,YOffset=0 WHERE (PlanID == '%q')",
					idx.c_str()
				);
				_log.Log(LOG_STATUS, "(Floorplan) Device coordinates reset for plan '%s'.", idx.c_str());
				m_sql.safe_query(
					"UPDATE Plans SET FloorplanID=0,Area='' WHERE (ID == '%q')",
					idx.c_str()
				);
				_log.Log(LOG_STATUS, "(Floorplan) Plan '%s' floorplan data reset.", idx.c_str());
			}
		}
