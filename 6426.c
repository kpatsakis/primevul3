void FurnaceGUI::drawPattern() {
  //int delta0=SDL_GetPerformanceCounter();
  if (nextWindow==GUI_WINDOW_PATTERN) {
    patternOpen=true;
    ImGui::SetNextWindowFocus();
    nextWindow=GUI_WINDOW_NOTHING;
  }
  if (!patternOpen) return;

  bool inhibitMenu=false;
  float scrollX=0;

  if (e->isPlaying() && followPattern) cursor.y=oldRow;
  demandX=0;
  sel1=selStart;
  sel2=selEnd;
  if (sel2.y<sel1.y) {
    sel2.y^=sel1.y;
    sel1.y^=sel2.y;
    sel2.y^=sel1.y;
  }
  if (sel2.xCoarse<sel1.xCoarse) {
    sel2.xCoarse^=sel1.xCoarse;
    sel1.xCoarse^=sel2.xCoarse;
    sel2.xCoarse^=sel1.xCoarse;

    sel2.xFine^=sel1.xFine;
    sel1.xFine^=sel2.xFine;
    sel2.xFine^=sel1.xFine;
  } else if (sel2.xCoarse==sel1.xCoarse && sel2.xFine<sel1.xFine) {
    sel2.xFine^=sel1.xFine;
    sel1.xFine^=sel2.xFine;
    sel2.xFine^=sel1.xFine;
  }
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(0.0f,0.0f));
  if (ImGui::Begin("Pattern",&patternOpen,settings.avoidRaisingPattern?ImGuiWindowFlags_NoBringToFrontOnFocus:0)) {
    //ImGui::SetWindowSize(ImVec2(scrW*dpiScale,scrH*dpiScale));
    patWindowPos=ImGui::GetWindowPos();
    patWindowSize=ImGui::GetWindowSize();
    //char id[32];
    ImGui::PushFont(patFont);
    int ord=e->isPlaying()?oldOrder:e->getOrder();
    oldOrder=e->getOrder();
    int chans=e->getTotalChannelCount();
    int displayChans=0;
    const DivPattern* patCache[DIV_MAX_CHANS];
    for (int i=0; i<chans; i++) {
      if (e->song.chanShow[i]) displayChans++;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding,ImVec2(0.0f,0.0f));
    ImGui::PushStyleColor(ImGuiCol_Header,uiColors[GUI_COLOR_PATTERN_SELECTION]);
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered,uiColors[GUI_COLOR_PATTERN_SELECTION_HOVER]);
    ImGui::PushStyleColor(ImGuiCol_HeaderActive,uiColors[GUI_COLOR_PATTERN_SELECTION_ACTIVE]);
    if (ImGui::BeginTable("PatternView",displayChans+2,ImGuiTableFlags_BordersInnerV|ImGuiTableFlags_ScrollX|ImGuiTableFlags_ScrollY|ImGuiTableFlags_NoPadInnerX)) {
      ImGui::TableSetupColumn("pos",ImGuiTableColumnFlags_WidthFixed);
      char chanID[2048];
      float lineHeight=(ImGui::GetTextLineHeight()+2*dpiScale);
      int curRow=e->getRow();
      if (e->isPlaying() && followPattern) updateScroll(curRow);
      if (nextScroll>-0.5f) {
        ImGui::SetScrollY(nextScroll);
        nextScroll=-1.0f;
        nextAddScroll=0.0f;
      }
      if (nextAddScroll!=0.0f) {
        ImGui::SetScrollY(ImGui::GetScrollY()+nextAddScroll);
        nextScroll=-1.0f;
        nextAddScroll=0.0f;
      }
      ImGui::TableSetupScrollFreeze(1,1);
      for (int i=0; i<chans; i++) {
        if (!e->song.chanShow[i]) continue;
        ImGui::TableSetupColumn(fmt::sprintf("c%d",i).c_str(),ImGuiTableColumnFlags_WidthFixed);
      }
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      if (ImGui::Selectable((extraChannelButtons==2)?" --##ExtraChannelButtons":" ++##ExtraChannelButtons",false,ImGuiSelectableFlags_NoPadWithHalfSpacing,ImVec2(0.0f,lineHeight+1.0f*dpiScale))) {
        if (++extraChannelButtons>2) extraChannelButtons=0;
      }
      if (ImGui::IsItemHovered()) {
        if (extraChannelButtons==2) {
          ImGui::SetTooltip("Pattern names (click to collapse)\nRight-click for visualizer");
        } else if (extraChannelButtons==1) {
          ImGui::SetTooltip("Expanded (click for pattern names)\nRight-click for visualizer");
        } else {
          ImGui::SetTooltip("Compact (click to expand)\nRight-click for visualizer");
        }
      }
      if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        fancyPattern=!fancyPattern;
        inhibitMenu=true;
        e->enableCommandStream(fancyPattern);
        e->getCommandStream(cmdStream);
        cmdStream.clear();
      }
      for (int i=0; i<chans; i++) {
        if (!e->song.chanShow[i]) continue;
        ImGui::TableNextColumn();
        bool displayTooltip=false;
        if (e->song.chanCollapse[i]) {
          const char* chName=e->getChannelShortName(i);
          if (strlen(chName)>3) {
            snprintf(chanID,2048,"...##_CH%d",i);
          } else {
            snprintf(chanID,2048,"%s##_CH%d",chName,i);
          }
          displayTooltip=true;
        } else {
          const char* chName=e->getChannelName(i);
          size_t chNameLimit=6+4*e->song.pat[i].effectRows;
          if (strlen(chName)>chNameLimit) {
            String shortChName=chName;
            shortChName.resize(chNameLimit-3);
            shortChName+="...";
            snprintf(chanID,2048," %s##_CH%d",shortChName.c_str(),i);
            displayTooltip=true;
          } else {
            snprintf(chanID,2048," %s##_CH%d",chName,i);
          }
        }
        bool muted=e->isChannelMuted(i);
        ImVec4 chanHead=muted?uiColors[GUI_COLOR_CHANNEL_MUTED]:uiColors[GUI_COLOR_CHANNEL_FM+e->getChannelType(i)];
        ImVec4 chanHeadActive=chanHead;
        ImVec4 chanHeadHover=chanHead;
        if (e->keyHit[i]) {
          keyHit[i]=0.2;
          e->keyHit[i]=false;
        }
        if (settings.guiColorsBase) {
          chanHead.x*=1.0-keyHit[i]; chanHead.y*=1.0-keyHit[i]; chanHead.z*=1.0-keyHit[i];
          chanHeadActive.x*=0.5; chanHeadActive.y*=0.5; chanHeadActive.z*=0.5;
          chanHeadHover.x*=0.9-keyHit[i]; chanHeadHover.y*=0.9-keyHit[i]; chanHeadHover.z*=0.9-keyHit[i];
        } else {
          chanHead.x*=0.25+keyHit[i]; chanHead.y*=0.25+keyHit[i]; chanHead.z*=0.25+keyHit[i];
          chanHeadActive.x*=0.8; chanHeadActive.y*=0.8; chanHeadActive.z*=0.8;
          chanHeadHover.x*=0.4+keyHit[i]; chanHeadHover.y*=0.4+keyHit[i]; chanHeadHover.z*=0.4+keyHit[i];
        }
        keyHit[i]-=0.02*60.0*ImGui::GetIO().DeltaTime;
        if (keyHit[i]<0) keyHit[i]=0;
        ImGui::PushStyleColor(ImGuiCol_Header,chanHead);
        ImGui::PushStyleColor(ImGuiCol_HeaderActive,chanHeadActive);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered,chanHeadHover);
        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg,ImGui::GetColorU32(chanHead));
        if (muted) ImGui::PushStyleColor(ImGuiCol_Text,uiColors[GUI_COLOR_CHANNEL_MUTED]);
        ImGui::Selectable(chanID,true,ImGuiSelectableFlags_NoPadWithHalfSpacing,ImVec2(0.0f,lineHeight+1.0f*dpiScale));
        if (displayTooltip && ImGui::IsItemHovered()) {
          ImGui::SetTooltip("%s",e->getChannelName(i));
        }
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
          if (settings.soloAction!=1 && soloTimeout>0 && soloChan==i) {
            e->toggleSolo(i);
            soloTimeout=0;
          } else {
            e->toggleMute(i);
            soloTimeout=20;
            soloChan=i;
          }
        }
        if (muted) ImGui::PopStyleColor();
        ImGui::PopStyleColor(3);
        if (settings.soloAction!=2) if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
          inhibitMenu=true;
          e->toggleSolo(i);
        }
        if (extraChannelButtons==2) {
          DivPattern* pat=e->song.pat[i].getPattern(e->song.orders.ord[i][ord],true);
          ImGui::PushFont(mainFont);
          if (patNameTarget==i) {
            snprintf(chanID,2048,"##PatNameI%d_%d",i,ord);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x-(8.0f*dpiScale));
            ImGui::SetCursorPosX(ImGui::GetCursorPosX()+4.0f*dpiScale);
            ImGui::InputText(chanID,&pat->name);
            if (wantPatName) {
              wantPatName=false;
              ImGui::SetItemDefaultFocus();
              ImGui::SetKeyboardFocusHere(-1);
            } else {
              if (!ImGui::IsItemActive()) {
                patNameTarget=-1;
              }
            }
          } else {
            snprintf(chanID,2048," %s##PatName%d",pat->name.c_str(),i);
            if (ImGui::Selectable(chanID,true,ImGuiSelectableFlags_NoPadWithHalfSpacing,ImVec2(0.0f,lineHeight+1.0f*dpiScale))) {
              patNameTarget=i;
              wantPatName=true;
              snprintf(chanID,2048,"##PatNameI%d_%d",i,ord);
              ImGui::SetActiveID(ImGui::GetID(chanID),ImGui::GetCurrentWindow());
            }
          }
          ImGui::PopFont();
        } else if (extraChannelButtons==1) {
          snprintf(chanID,2048,"%c##_HCH%d",e->song.chanCollapse[i]?'+':'-',i);
          ImGui::SetCursorPosX(ImGui::GetCursorPosX()+4.0f*dpiScale);
          if (ImGui::SmallButton(chanID)) {
            e->song.chanCollapse[i]=!e->song.chanCollapse[i];
          }
          if (!e->song.chanCollapse[i]) {
            ImGui::SameLine();
            snprintf(chanID,2048,"<##_LCH%d",i);
            ImGui::BeginDisabled(e->song.pat[i].effectRows<=1);
            if (ImGui::SmallButton(chanID)) {
              e->song.pat[i].effectRows--;
              if (e->song.pat[i].effectRows<1) e->song.pat[i].effectRows=1;
            }
            ImGui::EndDisabled();
            ImGui::SameLine();
            ImGui::BeginDisabled(e->song.pat[i].effectRows>=8);
            snprintf(chanID,2048,">##_RCH%d",i);
            if (ImGui::SmallButton(chanID)) {
              e->song.pat[i].effectRows++;
              if (e->song.pat[i].effectRows>8) e->song.pat[i].effectRows=8;
            }
            ImGui::EndDisabled();
          }
          ImGui::Spacing();
        }
      }
      ImGui::TableNextColumn();
      if (e->hasExtValue()) {
        ImGui::TextColored(uiColors[GUI_COLOR_EE_VALUE]," %.2X",e->getExtValue());
      }
      float oneCharSize=ImGui::CalcTextSize("A").x;
      threeChars=ImVec2(oneCharSize*3.0f,lineHeight);
      twoChars=ImVec2(oneCharSize*2.0f,lineHeight);
      //ImVec2 oneChar=ImVec2(oneCharSize,lineHeight);
      dummyRows=(ImGui::GetWindowSize().y/lineHeight)/2;
      // オップナー2608 i owe you one more for this horrible code
      // previous pattern
      ImGui::BeginDisabled();
      if (settings.viewPrevPattern) {
        if ((ord-1)>=0) for (int i=0; i<chans; i++) {
          patCache[i]=e->song.pat[i].getPattern(e->song.orders.ord[i][ord-1],true);
        }
        for (int i=0; i<dummyRows-1; i++) {
          patternRow(e->song.patLen+i-dummyRows+1,e->isPlaying(),lineHeight,chans,ord-1,patCache);
        }
      } else {
        for (int i=0; i<dummyRows-1; i++) {
          ImGui::TableNextRow(0,lineHeight);
          ImGui::TableNextColumn();
        }
      }
      ImGui::EndDisabled();
      // active area
      for (int i=0; i<chans; i++) {
        patCache[i]=e->song.pat[i].getPattern(e->song.orders.ord[i][ord],true);
      }
      for (int i=0; i<e->song.patLen; i++) {
        patternRow(i,e->isPlaying(),lineHeight,chans,ord,patCache);
      }
      // next pattern
      ImGui::BeginDisabled();
      if (settings.viewPrevPattern) {
        if ((ord+1)<e->song.ordersLen) for (int i=0; i<chans; i++) {
          patCache[i]=e->song.pat[i].getPattern(e->song.orders.ord[i][ord+1],true);
        }
        for (int i=0; i<=dummyRows; i++) {
          patternRow(i,e->isPlaying(),lineHeight,chans,ord+1,patCache);
        }
      } else {
        for (int i=0; i<=dummyRows; i++) {
          ImGui::TableNextRow(0,lineHeight);
          ImGui::TableNextColumn();
        }
      }
      ImGui::EndDisabled();
      oldRow=curRow;
      if (demandScrollX) {
        int totalDemand=demandX-ImGui::GetScrollX();
        if (totalDemand<80) {
          ImGui::SetScrollX(demandX-200*dpiScale);
        } else if (totalDemand>(ImGui::GetWindowWidth()-200*dpiScale)) {
          ImGui::SetScrollX(demandX+200*dpiScale);
        }
        demandScrollX=false;
      }
      scrollX=ImGui::GetScrollX();
      ImGui::EndTable();
    }

    if (fancyPattern) { // visualizer
      e->getCommandStream(cmdStream);
      ImDrawList* dl=ImGui::GetWindowDrawList();
      ImVec2 off=ImGui::GetWindowPos();
      
      // commands
      for (DivCommand& i: cmdStream) {
        if (i.cmd==DIV_CMD_PITCH) continue;
        if (i.cmd==DIV_CMD_NOTE_PORTA) continue;
        //if (i.cmd==DIV_CMD_NOTE_ON) continue;
        if (i.cmd==DIV_CMD_PRE_PORTA) continue;
        if (i.cmd==DIV_CMD_PRE_NOTE) continue;
        if (i.cmd==DIV_CMD_SAMPLE_BANK) continue;
        if (i.cmd==DIV_CMD_GET_VOLUME) continue;
        if (i.cmd==DIV_ALWAYS_SET_VOLUME) continue;

        float width=patChanX[i.chan+1]-patChanX[i.chan];
        float speedX=0.0f;
        float speedY=-18.0f;
        float grav=0.6f;
        float frict=1.0f;
        float life=255.0f;
        float lifeSpeed=8.0f;
        float spread=5.0f;
        int num=3;
        const char* partIcon=ICON_FA_MICROCHIP;
        ImU32* color=noteGrad;

        switch (i.cmd) {
          case DIV_CMD_NOTE_ON:
            partIcon=ICON_FA_ASTERISK;
            life=96.0f;
            lifeSpeed=3.0f;
            break;
          case DIV_CMD_LEGATO:
            partIcon=ICON_FA_COG;
            color=insGrad;
            life=64.0f;
            lifeSpeed=2.0f;
            break;
          case DIV_CMD_NOTE_OFF:
          case DIV_CMD_NOTE_OFF_ENV:
          case DIV_CMD_ENV_RELEASE:
            partIcon=ICON_FA_ASTERISK;
            speedX=0.0f;
            speedY=0.0f;
            grav=0.0f;
            life=24.0f;
            lifeSpeed=4.0f;
            break;
          case DIV_CMD_VOLUME: {
            float scaledVol=(float)i.value/(float)e->getMaxVolumeChan(i.chan);
            if (scaledVol>1.0f) scaledVol=1.0f;
            speedY=-18.0f-(10.0f*scaledVol);
            life=128+scaledVol*127;
            partIcon=ICON_FA_VOLUME_UP;
            num=12.0f*pow(scaledVol,2.0);
            color=volGrad;
            break;
          }
          case DIV_CMD_INSTRUMENT: {
            if (lastIns[i.chan]==i.value) {
              num=0;
              break;
            }
            lastIns[i.chan]=i.value;
            speedX=0.0f;
            speedY=0.0f;
            grav=0.0f;
            frict=0.98;
            spread=30.0f;
            life=128.0f;
            lifeSpeed=6.0f;
            color=insGrad;
            num=7+pow(i.value,0.6);
            break;
          }
          case DIV_CMD_PANNING: {
            if (i.value==0) {
              num=0;
              break;
            }
            float ratio=float(((i.value>>4)&15)-(i.value&15))/MAX(((i.value>>4)&15),(i.value&15));
            speedX=-22.0f*sin(ratio*M_PI*0.5);
            speedY=-22.0f*cos(ratio*M_PI*0.5);
            spread=5.0f+fabs(sin(ratio*M_PI*0.5))*7.0f;
            grav=0.0f;
            frict=0.96f;
            if (((i.value>>4)&15)==(i.value&15)) {
              partIcon=ICON_FA_ARROWS_H;
            } else if (ratio>0) {
              partIcon=ICON_FA_ARROW_LEFT;
            } else {
              partIcon=ICON_FA_ARROW_RIGHT;
            }
            num=9;
            color=panGrad;
            break;
          }
          case DIV_CMD_SAMPLE_FREQ:
            speedX=0.0f;
            speedY=0.0f;
            grav=0.0f;
            frict=0.98;
            spread=19.0f;
            life=128.0f;
            lifeSpeed=3.0f;
            color=sysCmd2Grad;
            num=10+pow(i.value,0.6);
            break;
          default:
            //printf("unhandled %d\n",i.cmd);
            color=sysCmd1Grad;
            break;
        }

        for (int j=0; j<num; j++) {
          particles.push_back(Particle(
            color,
            partIcon,
            off.x+patChanX[i.chan]+fmod(rand(),width)-scrollX,
            off.y+(ImGui::GetWindowHeight()*0.5f)+randRange(0,patFont->FontSize),
            (speedX+randRange(-spread,spread))*0.5*dpiScale,
            (speedY+randRange(-spread,spread))*0.5*dpiScale,
            grav,
            frict,
            life-randRange(0,8),
            lifeSpeed
          ));
        }
      }

      float frameTime=ImGui::GetIO().DeltaTime*60.0f;

      // note slides
      ImVec2 arrowPoints[7];
      if (e->isPlaying()) for (int i=0; i<chans; i++) {
        if (!e->song.chanShow[i]) continue;
        DivChannelState* ch=e->getChanState(i);
        if (ch->portaSpeed>0) {
          ImVec4 col=uiColors[GUI_COLOR_PATTERN_EFFECT_PITCH];
          col.w*=0.2;
          float width=patChanX[i+1]-patChanX[i];

          particles.push_back(Particle(
            pitchGrad,
            (ch->portaNote<=ch->note)?ICON_FA_CHEVRON_DOWN:ICON_FA_CHEVRON_UP,
            off.x+patChanX[i]+fmod(rand(),width)-scrollX,
            off.y+fmod(rand(),MAX(1,ImGui::GetWindowHeight())),
            0.0f,
            (7.0f+(rand()%5)+pow(ch->portaSpeed,0.7f))*((ch->portaNote<=ch->note)?1:-1),
            0.0f,
            1.0f,
            255.0f,
            15.0f
          ));

          if (width>0.1) for (float j=-patChanSlideY[i]; j<ImGui::GetWindowHeight(); j+=width*0.7) {
            ImVec2 tMin=ImVec2(off.x+patChanX[i]-scrollX,off.y+j);
            ImVec2 tMax=ImVec2(off.x+patChanX[i+1]-scrollX,off.y+j+width*0.6);
            if (ch->portaNote<=ch->note) {
              arrowPoints[0]=ImLerp(tMin,tMax,ImVec2(0.1,1.0-0.8));
              arrowPoints[1]=ImLerp(tMin,tMax,ImVec2(0.5,1.0-0.0));
              arrowPoints[2]=ImLerp(tMin,tMax,ImVec2(0.9,1.0-0.8));
              arrowPoints[3]=ImLerp(tMin,tMax,ImVec2(0.8,1.0-1.0));
              arrowPoints[4]=ImLerp(tMin,tMax,ImVec2(0.5,1.0-0.37));
              arrowPoints[5]=ImLerp(tMin,tMax,ImVec2(0.2,1.0-1.0));
              arrowPoints[6]=arrowPoints[0];
              dl->AddPolyline(arrowPoints,7,ImGui::GetColorU32(col),ImDrawFlags_None,5.0f*dpiScale);
            } else {
              arrowPoints[0]=ImLerp(tMin,tMax,ImVec2(0.1,0.8));
              arrowPoints[1]=ImLerp(tMin,tMax,ImVec2(0.5,0.0));
              arrowPoints[2]=ImLerp(tMin,tMax,ImVec2(0.9,0.8));
              arrowPoints[3]=ImLerp(tMin,tMax,ImVec2(0.8,1.0));
              arrowPoints[4]=ImLerp(tMin,tMax,ImVec2(0.5,0.37));
              arrowPoints[5]=ImLerp(tMin,tMax,ImVec2(0.2,1.0));
              arrowPoints[6]=arrowPoints[0];
              dl->AddPolyline(arrowPoints,7,ImGui::GetColorU32(col),ImDrawFlags_None,5.0f*dpiScale);
            }
          }
          patChanSlideY[i]+=((ch->portaNote<=ch->note)?-8:8)*dpiScale*frameTime;
          if (width>0) {
            if (patChanSlideY[i]<0) {
              patChanSlideY[i]=-fmod(-patChanSlideY[i],width*0.7);
            } else {
              patChanSlideY[i]=fmod(patChanSlideY[i],width*0.7);
            }
          }
        }
      }

      // particle simulation
      ImDrawList* fdl=ImGui::GetForegroundDrawList();
      for (size_t i=0; i<particles.size(); i++) {
        Particle& part=particles[i];
        if (part.update(frameTime)) {
          if (part.life>255) part.life=255;
          fdl->AddText(
            iconFont,
            iconFont->FontSize,
            ImVec2(part.pos.x-iconFont->FontSize*0.5,part.pos.y-iconFont->FontSize*0.5),
            part.colors[(int)part.life],
            part.type
          );
        } else {
          particles.erase(particles.begin()+i);
          i--;
        }
      }
    }

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
    ImGui::PopFont();
  }
  ImGui::PopStyleVar();
  if (patternOpen) {
    if (!inhibitMenu && ImGui::IsItemClicked(ImGuiMouseButton_Right)) ImGui::OpenPopup("patternActionMenu");
    if (ImGui::BeginPopup("patternActionMenu",ImGuiWindowFlags_NoMove|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoSavedSettings)) {
      editOptions(false);
      ImGui::EndPopup();
    }
  }
  if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows)) curWindow=GUI_WINDOW_PATTERN;
  ImGui::End();
  //int delta1=SDL_GetPerformanceCounter();
  //logV("render time: %dµs\n",(delta1-delta0)/(SDL_GetPerformanceFrequency()/1000000));
}