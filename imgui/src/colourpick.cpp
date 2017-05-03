#include "imgui.h"
#include "imgui_internal.h"
#include <glm/glm.hpp>

// based on https://gist.github.com/thennequin/a21d5769bdcaa4d0992e

ImVec4 glmColorToImVec4(const glm::vec4 &_c)
{
  return ImVec4(_c.r,_c.g,_c.b,_c.a);
}

bool ColorSelector(const char* pLabel, glm::vec4 &oRGBA)
{
  const ImU32 c_oColorGrey = ImGui::ColorConvertFloat4ToU32(ImVec4(0.75f,0.75f,0.75f,1.f));
  const ImU32 c_oColorBlack = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f,0.f,0.f,1.f));
  const ImU32 c_oColorBlackTransparent = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f,0.f,0.f,0.f));
  const ImU32 c_oColorWhite = ImGui::ColorConvertFloat4ToU32(ImVec4(1.f,1.f,1.f,1.f));

  ImGui::PushID(pLabel);
  bool bRet = false;
  ImGuiID iID = ImGui::GetID(pLabel);
  ImGuiWindow* pWindow = ImGui::GetCurrentWindow();

  const ImGuiID iStorageOpen = iID + ImGui::GetID("ColorSelector_Open");

  const ImGuiID iStorageStartColorR = iID + ImGui::GetID("ColorSelector_StartColor_R");
  const ImGuiID iStorageStartColorG = iID + ImGui::GetID("ColorSelector_StartColor_G");
  const ImGuiID iStorageStartColorB = iID + ImGui::GetID("ColorSelector_StartColor_B");

  const ImGuiID iStorageCurrentColorH = iID + ImGui::GetID("ColorSelector_CurrentColor_H");
  const ImGuiID iStorageCurrentColorS = iID + ImGui::GetID("ColorSelector_CurrentColor_S");
  const ImGuiID iStorageCurrentColorV = iID + ImGui::GetID("ColorSelector_CurrentColor_V");


  //pWindow->StateStorage.SetFloat(iID);

  ImDrawList* pDrawList = ImGui::GetWindowDrawList();

  if (ImGui::InvisibleButton("Picker", ImVec2(16,16)))
  {
    pWindow->StateStorage.SetInt(iStorageOpen, 1);

    pWindow->StateStorage.SetFloat(iStorageStartColorR, oRGBA.r);
    pWindow->StateStorage.SetFloat(iStorageStartColorG, oRGBA.g);
    pWindow->StateStorage.SetFloat(iStorageStartColorB, oRGBA.b);

    float fHue, fSat, fVal;
    ImGui::ColorConvertRGBtoHSV( oRGBA.r, oRGBA.g, oRGBA.b, fHue, fSat, fVal );

    pWindow->StateStorage.SetFloat(iStorageCurrentColorH, fHue);
    pWindow->StateStorage.SetFloat(iStorageCurrentColorS, fSat);
    pWindow->StateStorage.SetFloat(iStorageCurrentColorV, fVal);
  }

  for (int iX = 0; iX < 2; ++iX)
  {
    for (int iY = 0; iY < 2; ++iY)
    {
      ImVec2 oA(ImGui::GetItemRectMin().x + iX * 8.f, ImGui::GetItemRectMin().y + iY * 8.f);
      ImVec2 oB(ImGui::GetItemRectMin().x + (1+iX) * 8.f, ImGui::GetItemRectMin().y + (1+iY) * 8.f);
      pDrawList->AddRectFilled( oA, oB, (0 == (iX+iY)%2) ? c_oColorGrey : c_oColorWhite );
    }
  }
  pDrawList->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImGui::ColorConvertFloat4ToU32(glmColorToImVec4(oRGBA)));

  ImVec2 oRectMin = ImGui::GetItemRectMin();
  ImVec2 oRectMax = ImGui::GetItemRectMax();

  const ImVec2 oPopupSize(175,350);
  ImGui::SetNextWindowSize(oPopupSize, ImGuiSetCond_Always);
  ImGui::SetNextWindowPos(ImVec2(oRectMin.x, oRectMax.y + 5), ImGuiSetCond_Appearing);
  if (pWindow->StateStorage.GetInt(iStorageOpen, 0) == 1 )
  {
    ImGui::Begin(pLabel, NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    bRet = false;
    const int iCheckboardTileSize = 10;

    ImDrawList* pDrawList = ImGui::GetWindowDrawList();

    ImVec2 oColorPreviewSize(160, 20);
    ImGui::Dummy(oColorPreviewSize);
    ImVec2 oColorAreaMin = ImGui::GetItemRectMin();
    ImVec2 oColorAreaMax = ImGui::GetItemRectMax();

    int iTileHCount = (int)oColorPreviewSize.x / iCheckboardTileSize;
    int iTileVCount = (int)oColorPreviewSize.y / iCheckboardTileSize;

    for (int iX = 0; iX < iTileHCount; ++iX)
    {
      for (int iY = 0; iY < iTileVCount; ++iY)
      {
        pDrawList->AddRectFilled(
          ImVec2(oColorAreaMin.x + iX * iCheckboardTileSize, oColorAreaMin.y + iY * iCheckboardTileSize),
          ImVec2(oColorAreaMin.x + (1+iX) * iCheckboardTileSize, oColorAreaMin.y + (1+iY) * iCheckboardTileSize),
          (0 == (iX+iY)%2) ? c_oColorGrey : c_oColorWhite );
      }
    }

    pDrawList->AddRectFilled(oColorAreaMin, oColorAreaMax, ImGui::ColorConvertFloat4ToU32(glmColorToImVec4(oRGBA)));

    float fHue = pWindow->StateStorage.GetFloat(iStorageCurrentColorH);
    float fSat = pWindow->StateStorage.GetFloat(iStorageCurrentColorS);
    float fVal = pWindow->StateStorage.GetFloat(iStorageCurrentColorV);

    ImGui::Text("HSV");
    ImGui::Separator();
    {
      //Saturation
      {
        const ImVec2 oSaturationAreaSize(128,128);
        ImGui::InvisibleButton("##SaturationArea", oSaturationAreaSize);
        ImVec2 oSaturationAreaMin = ImGui::GetItemRectMin();
        ImVec2 oSaturationAreaMax = ImGui::GetItemRectMax();

        if (ImGui::IsItemActive())
        {
          bRet = true;
          ImVec2 oCursorPos = ImGui::GetMousePos();
          ImVec2 oNewValue((oCursorPos.x - oSaturationAreaMin.x) / oSaturationAreaSize.x, (oCursorPos.y - oSaturationAreaMin.y) / oSaturationAreaSize.y);
          oNewValue.x = ImClamp(oNewValue.x, 0.f, 1.f);
          oNewValue.y =ImClamp(oNewValue.y, 0.f, 1.f);
          fSat =  oNewValue.x;
          fVal =  1.f - oNewValue.y;
          ImGui::ColorConvertHSVtoRGB( fHue, fSat, fVal, oRGBA.r, oRGBA.g, oRGBA.b );

          ImVec4 oToolTipColor = glmColorToImVec4(oRGBA);
          oToolTipColor.w = 1.f;

          ImGui::BeginTooltip();
          ImGui::Dummy(ImVec2(32,32));
          ImVec2 oDummyAreaMin = ImGui::GetItemRectMin();
          ImVec2 oDummyAreaMax = ImGui::GetItemRectMax();
          ImDrawList* pDummyDrawList = ImGui::GetWindowDrawList();
          pDummyDrawList->AddRectFilled( oDummyAreaMin, oDummyAreaMax, ImGui::ColorConvertFloat4ToU32(oToolTipColor));
          ImGui::EndTooltip();
        }

        ImVec4 cHueValue(1, 1, 1, 1);
        ImGui::ColorConvertHSVtoRGB(fHue, 1, 1, cHueValue.x, cHueValue.y, cHueValue.z);
        ImU32 oHueColor = ImGui::ColorConvertFloat4ToU32(cHueValue);

        pDrawList->AddRectFilledMultiColor(
          oSaturationAreaMin,
          oSaturationAreaMax,
          c_oColorWhite,
          oHueColor,
          oHueColor,
          c_oColorWhite
          );

        pDrawList->AddRectFilledMultiColor(
          oSaturationAreaMin,
          oSaturationAreaMax,
          c_oColorBlackTransparent,
          c_oColorBlackTransparent,
          c_oColorBlack,
          c_oColorBlack
          );

        pDrawList->AddCircle(ImVec2(oSaturationAreaMin.x + oSaturationAreaSize.x * fSat, oSaturationAreaMin.y + oSaturationAreaSize.y * (1.f - fVal)), 4, c_oColorBlack, 6);
      }
      ImGui::SameLine();
      //Hue
      {
        const ImVec2 oHueAreaSize(20,128);
        ImGui::InvisibleButton("##HueArea", oHueAreaSize);
        //TODO tooltip
        ImVec2 oHueAreaMin = ImGui::GetItemRectMin();
        ImVec2 oHueAreaMax = ImGui::GetItemRectMax();

        if (ImGui::IsItemActive())
        {
          bRet = true;
          fHue = (ImGui::GetMousePos().y - oHueAreaMin.y) / oHueAreaSize.y;
          fHue = ImClamp(fHue, 0.f, 1.f);
          ImGui::ColorConvertHSVtoRGB( fHue, fSat, fVal, oRGBA.r, oRGBA.g, oRGBA.b );

          ImGui::BeginTooltip();
          ImGui::Dummy(ImVec2(32,32));
          ImVec2 oDummyAreaMin = ImGui::GetItemRectMin();
          ImVec2 oDummyAreaMax = ImGui::GetItemRectMax();
          ImDrawList* pDummyDrawList = ImGui::GetWindowDrawList();
          ImVec4 oNewHueRGB;
          oNewHueRGB.w = 1.f;
          ImGui::ColorConvertHSVtoRGB( fHue, 1.f, 1.f, oNewHueRGB.x, oNewHueRGB.y, oNewHueRGB.z );
          pDummyDrawList->AddRectFilled( oDummyAreaMin, oDummyAreaMax, ImGui::ColorConvertFloat4ToU32(oNewHueRGB));
          ImGui::EndTooltip();
        }

        ImVec4 c0(1, 1, 1, 1);
        ImVec4 c1(1, 1, 1, 1);

        const int iStepCount = 8;
        for (int iStep = 0; iStep < iStepCount; iStep++)
        {
          float h0 = (float)iStep / (float)iStepCount;
          float h1 = (float)(iStep + 1.f) / (float)iStepCount;
          ImGui::ColorConvertHSVtoRGB(h0, 1.f, 1.f, c0.x, c0.y, c0.z);
          ImGui::ColorConvertHSVtoRGB(h1, 1.f, 1.f, c1.x, c1.y, c1.z);

          pDrawList->AddRectFilledMultiColor(
            ImVec2(oHueAreaMin.x, oHueAreaMin.y + oHueAreaSize.y * h0),
            ImVec2(oHueAreaMax.x, oHueAreaMin.y + oHueAreaSize.y * h1),
            ImGui::ColorConvertFloat4ToU32(c0),
            ImGui::ColorConvertFloat4ToU32(c0),
            ImGui::ColorConvertFloat4ToU32(c1),
            ImGui::ColorConvertFloat4ToU32(c1)
            );
        }

        pDrawList->AddLine(
          ImVec2(oHueAreaMin.x, oHueAreaMin.y + oHueAreaSize.y * fHue),
          ImVec2(oHueAreaMax.x, oHueAreaMin.y + oHueAreaSize.y * fHue),
          c_oColorWhite
          );
      }
    }

    //RGBA Sliders
    ImGui::Text("RGBA");
    ImGui::Separator();
    {
      int r = (int)(ImSaturate( oRGBA.r )*255.f);
      int g = (int)(ImSaturate( oRGBA.g )*255.f);
      int b = (int)(ImSaturate( oRGBA.b )*255.f);
      bool bChange = false;
      ImGui::PushItemWidth(130.f);
      bChange |= ImGui::SliderInt("R", &r, 0, 255);
      bChange |= ImGui::SliderInt("G", &g, 0, 255);
      bChange |= ImGui::SliderInt("B", &b, 0, 255);
      ImGui::PopItemWidth();
      if (bChange)
      {
        bRet = true;
        oRGBA.r = (float)r/255.f;
        oRGBA.g = (float)g/255.f;
        oRGBA.b = (float)b/255.f;

        ImGui::ColorConvertRGBtoHSV( oRGBA.r, oRGBA.g, oRGBA.b, fHue, fSat, fVal );
      }
    }

    if (bRet)
    {
      pWindow->StateStorage.SetFloat(iStorageCurrentColorH, fHue);
      pWindow->StateStorage.SetFloat(iStorageCurrentColorS, fSat);
      pWindow->StateStorage.SetFloat(iStorageCurrentColorV, fVal);
    }

    if (ImGui::Button("Ok"))
    {
      pWindow->StateStorage.SetInt(iStorageOpen, 0);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
    {
      pWindow->StateStorage.SetInt(iStorageOpen, 0);
      oRGBA.r = pWindow->StateStorage.GetFloat(iStorageStartColorR);
      oRGBA.g = pWindow->StateStorage.GetFloat(iStorageStartColorG);
      oRGBA.b = pWindow->StateStorage.GetFloat(iStorageStartColorB);

      bRet = true;
    }
    ImGui::End();
  }

  ImGui::SameLine();

  float fValues[3] = {oRGBA.r, oRGBA.g, oRGBA.b};
  if (ImGui::DragFloat3(pLabel, fValues, 0.01f))
  {
    oRGBA.r = fValues[0];
    oRGBA.g = fValues[1];
    oRGBA.b = fValues[2];
    bRet = true;
  }

  ImGui::PopID();
  return bRet;
}

bool ColorSelector(const char* pLabel, glm::vec3 &oRGB)
{
  glm::vec4 newRGB(oRGB,1.0f);
  bool bRet = ColorSelector(pLabel,newRGB);
  oRGB.r=newRGB.r;
  oRGB.g=newRGB.g;
  oRGB.b=newRGB.b;
  return bRet;
}
