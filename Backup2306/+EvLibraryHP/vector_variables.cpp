///////////////////////////////////////////////////////////////////////////////
// File generated by HDevelop for HALCON/C++ Version 12.0.2
///////////////////////////////////////////////////////////////////////////////



#ifndef __APPLE__
#  include "HalconCpp.h"
#  include "HDevThread.h"
#  if defined(__linux__) && !defined(NO_EXPORT_APP_MAIN)
#    include <X11/Xlib.h>
#  endif
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#    include <HALCONCpp/HDevThread.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#    include <HALCONCppxl/HDevThread.h>
#  endif
#  include <stdio.h>
#  include <HALCON/HpThread.h>
#  include <CoreFoundation/CFRunLoop.h>
#endif



using namespace HalconCpp;

// Procedure declarations 
// External procedures 
// Chapter: Develop
// Short Description: Switch dev_update_pc, dev_update_var and dev_update_window to 'off'. 
void dev_update_off ();
// Chapter: Graphics / Text
// Short Description: This procedure writes a text message. 
void disp_message (HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem, 
    HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box);
// Chapter: Graphics / Text
// Short Description: Set font independent of OS 
void set_display_font (HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold, 
    HTuple hv_Slant);

// Procedures 
// External procedures 
// Chapter: Develop
// Short Description: Switch dev_update_pc, dev_update_var and dev_update_window to 'off'. 
void dev_update_off ()
{

  //This procedure sets different update settings to 'off'.
  //This is useful to get the best performance and reduce overhead.
  //
  // dev_update_pc(...); only in hdevelop
  // dev_update_var(...); only in hdevelop
  // dev_update_window(...); only in hdevelop
  return;
}

// Chapter: Graphics / Text
// Short Description: This procedure writes a text message. 
void disp_message (HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem, 
    HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_M, hv_N, hv_Red, hv_Green, hv_Blue;
  HTuple  hv_RowI1Part, hv_ColumnI1Part, hv_RowI2Part, hv_ColumnI2Part;
  HTuple  hv_RowIWin, hv_ColumnIWin, hv_WidthWin, hv_HeightWin;
  HTuple  hv_I, hv_RowI, hv_ColumnI, hv_StringI, hv_MaxAscent;
  HTuple  hv_MaxDescent, hv_MaxWidth, hv_MaxHeight, hv_R1;
  HTuple  hv_C1, hv_FactorRowI, hv_FactorColumnI, hv_UseShadow;
  HTuple  hv_ShadowColor, hv_Exception, hv_Width, hv_Index;
  HTuple  hv_Ascent, hv_Descent, hv_W, hv_H, hv_FrameHeight;
  HTuple  hv_FrameWidth, hv_R2, hv_C2, hv_DrawMode, hv_CurrentColor;

  //This procedure displays text in a graphics window.
  //
  //Input parameters:
  //WindowHandle: The WindowHandle of the graphics window, where
  //   the message should be displayed
  //String: A tuple of strings containing the text message to be displayed
  //CoordSystem: If set to 'window', the text position is given
  //   with respect to the window coordinate system.
  //   If set to 'image', image coordinates are used.
  //   (This may be useful in zoomed images.)
  //Row: The row coordinate of the desired text position
  //   If set to -1, a default value of 12 is used.
  //   A tuple of values is allowed to display text at different
  //   positions.
  //Column: The column coordinate of the desired text position
  //   If set to -1, a default value of 12 is used.
  //   A tuple of values is allowed to display text at different
  //   positions.
  //Color: defines the color of the text as string.
  //   If set to [], '' or 'auto' the currently set color is used.
  //   If a tuple of strings is passed, the colors are used cyclically...
  //   - if |Row| == |Column| == 1: for each new textline
  //   = else for each text position.
  //Box: If Box[0] is set to 'true', the text is written within an orange box.
  //     If set to' false', no box is displayed.
  //     If set to a color string (e.g. 'white', '#FF00CC', etc.),
  //       the text is written in a box of that color.
  //     An optional second value for Box (Box[1]) controls if a shadow is displayed:
  //       'true' -> display a shadow in a default color
  //       'false' -> display no shadow
  //       otherwise -> use given string as color string for the shadow color
  //
  //It is possible to display multiple text strings in a single call.
  //In this case, some restrictions apply:
  //- Multiple text positions can be defined by specifying a tuple
  //  with multiple Row and/or Column coordinates, i.e.:
  //  - |Row| == n, |Column| == n
  //  - |Row| == n, |Column| == 1
  //  - |Row| == 1, |Column| == n
  //- If |Row| == |Column| == 1,
  //  each element of String is display in a new textline.
  //- If multiple positions or specified, the number of Strings
  //  must match the number of positions, i.e.:
  //  - Either |String| == n (each string is displayed at the
  //                          corresponding position),
  //  - or     |String| == 1 (The string is displayed n times).
  //
  if (0 != (hv_Color==HTuple()))
  {
    hv_Color = "";
  }
  if (0 != (hv_Box==HTuple()))
  {
    hv_Box = "false";
  }
  //
  //
  //Check conditions
  //
  hv_M = (hv_Row.TupleLength())*(hv_Column.TupleLength());
  hv_N = hv_Row.TupleLength();
  if (0 != (HTuple(hv_M==0).TupleOr(hv_String==HTuple())))
  {
    return;
  }
  if (0 != (hv_M!=1))
  {
    //Multiple positions
    //
    //Expand single parameters
    if (0 != ((hv_Row.TupleLength())==1))
    {
      hv_N = hv_Column.TupleLength();
      TupleGenConst(hv_N, hv_Row, &hv_Row);
    }
    else if (0 != ((hv_Column.TupleLength())==1))
    {
      TupleGenConst(hv_N, hv_Column, &hv_Column);
    }
    else if (0 != ((hv_Column.TupleLength())!=(hv_Row.TupleLength())))
    {
      throw HalconCpp::HException("Number of elements in Row and Column does not match.");
    }
    if (0 != ((hv_String.TupleLength())==1))
    {
      TupleGenConst(hv_N, hv_String, &hv_String);
    }
    else if (0 != ((hv_String.TupleLength())!=hv_N))
    {
      throw HalconCpp::HException("Number of elements in Strings does not match number of positions.");
    }
    //
  }
  //
  //Prepare window
  GetRgb(hv_WindowHandle, &hv_Red, &hv_Green, &hv_Blue);
  GetPart(hv_WindowHandle, &hv_RowI1Part, &hv_ColumnI1Part, &hv_RowI2Part, &hv_ColumnI2Part);
  GetWindowExtents(hv_WindowHandle, &hv_RowIWin, &hv_ColumnIWin, &hv_WidthWin, &hv_HeightWin);
  SetPart(hv_WindowHandle, 0, 0, hv_HeightWin-1, hv_WidthWin-1);
  //
  //Loop over all positions
  {
  HTuple end_val89 = hv_N-1;
  HTuple step_val89 = 1;
  for (hv_I=0; hv_I.Continue(end_val89, step_val89); hv_I += step_val89)
  {
    hv_RowI = HTuple(hv_Row[hv_I]);
    hv_ColumnI = HTuple(hv_Column[hv_I]);
    //Allow multiple strings for a single position.
    if (0 != (hv_N==1))
    {
      hv_StringI = hv_String;
    }
    else
    {
      //In case of multiple positions, only single strings
      //are allowed per position.
      //For line breaks, use \n in this case.
      hv_StringI = HTuple(hv_String[hv_I]);
    }
    //Default settings
    //-1 is mapped to 12.
    if (0 != (hv_RowI==-1))
    {
      hv_RowI = 12;
    }
    if (0 != (hv_ColumnI==-1))
    {
      hv_ColumnI = 12;
    }
    //
    //Split string into one string per line.
    hv_StringI = ((""+hv_StringI)+"").TupleSplit("\n");
    //
    //Estimate extentions of text depending on font size.
    GetFontExtents(hv_WindowHandle, &hv_MaxAscent, &hv_MaxDescent, &hv_MaxWidth, 
        &hv_MaxHeight);
    if (0 != (hv_CoordSystem==HTuple("window")))
    {
      hv_R1 = hv_RowI;
      hv_C1 = hv_ColumnI;
    }
    else
    {
      //Transform image to window coordinates.
      hv_FactorRowI = (1.*hv_HeightWin)/((hv_RowI2Part-hv_RowI1Part)+1);
      hv_FactorColumnI = (1.*hv_WidthWin)/((hv_ColumnI2Part-hv_ColumnI1Part)+1);
      hv_R1 = (((hv_RowI-hv_RowI1Part)+0.5)*hv_FactorRowI)-0.5;
      hv_C1 = (((hv_ColumnI-hv_ColumnI1Part)+0.5)*hv_FactorColumnI)-0.5;
    }
    //
    //Display text box depending on text size.
    hv_UseShadow = 1;
    hv_ShadowColor = "gray";
    if (0 != (HTuple(hv_Box[0])==HTuple("true")))
    {
      hv_Box[0] = "#fce9d4";
      hv_ShadowColor = "#f28d26";
    }
    if (0 != ((hv_Box.TupleLength())>1))
    {
      if (0 != (HTuple(hv_Box[1])==HTuple("true")))
      {
        //Use default ShadowColor set above
      }
      else if (0 != (HTuple(hv_Box[1])==HTuple("false")))
      {
        hv_UseShadow = 0;
      }
      else
      {
        hv_ShadowColor = ((const HTuple&)hv_Box)[1];
        //Valid color?
        try
        {
          SetColor(hv_WindowHandle, HTuple(hv_Box[1]));
        }
        // catch (Exception) 
        catch (HalconCpp::HException &HDevExpDefaultException)
        {
          HDevExpDefaultException.ToHTuple(&hv_Exception);
          hv_Exception = HTuple("Wrong value of control parameter Box[1] (must be a 'true', 'false', or a valid color string)");
          throw HalconCpp::HException(hv_Exception);
        }
      }
    }
    if (0 != (HTuple(hv_Box[0])!=HTuple("false")))
    {
      //Valid color?
      try
      {
        SetColor(hv_WindowHandle, HTuple(hv_Box[0]));
      }
      // catch (Exception) 
      catch (HalconCpp::HException &HDevExpDefaultException)
      {
        HDevExpDefaultException.ToHTuple(&hv_Exception);
        hv_Exception = HTuple("Wrong value of control parameter Box[0] (must be a 'true', 'false', or a valid color string)");
        throw HalconCpp::HException(hv_Exception);
      }
      //Calculate box extents
      hv_StringI = (" "+hv_StringI)+" ";
      hv_Width = HTuple();
      {
      HTuple end_val160 = (hv_StringI.TupleLength())-1;
      HTuple step_val160 = 1;
      for (hv_Index=0; hv_Index.Continue(end_val160, step_val160); hv_Index += step_val160)
      {
        GetStringExtents(hv_WindowHandle, HTuple(hv_StringI[hv_Index]), &hv_Ascent, 
            &hv_Descent, &hv_W, &hv_H);
        hv_Width = hv_Width.TupleConcat(hv_W);
      }
      }
      hv_FrameHeight = hv_MaxHeight*(hv_StringI.TupleLength());
      hv_FrameWidth = (HTuple(0).TupleConcat(hv_Width)).TupleMax();
      hv_R2 = hv_R1+hv_FrameHeight;
      hv_C2 = hv_C1+hv_FrameWidth;
      //Display rectangles
      GetDraw(hv_WindowHandle, &hv_DrawMode);
      SetDraw(hv_WindowHandle, "fill");
      //Set shadow color
      SetColor(hv_WindowHandle, hv_ShadowColor);
      if (0 != hv_UseShadow)
      {
        DispRectangle1(hv_WindowHandle, hv_R1+1, hv_C1+1, hv_R2+1, hv_C2+1);
      }
      //Set box color
      SetColor(hv_WindowHandle, HTuple(hv_Box[0]));
      DispRectangle1(hv_WindowHandle, hv_R1, hv_C1, hv_R2, hv_C2);
      SetDraw(hv_WindowHandle, hv_DrawMode);
    }
    //Write text.
    {
    HTuple end_val182 = (hv_StringI.TupleLength())-1;
    HTuple step_val182 = 1;
    for (hv_Index=0; hv_Index.Continue(end_val182, step_val182); hv_Index += step_val182)
    {
      //Set color
      if (0 != (hv_N==1))
      {
        //Wiht a single text position, each text line
        //may get a different color.
        hv_CurrentColor = HTuple(hv_Color[hv_Index%(hv_Color.TupleLength())]);
      }
      else
      {
        //With multiple text positions, each position
        //gets a single color for all text lines.
        hv_CurrentColor = HTuple(hv_Color[hv_I%(hv_Color.TupleLength())]);
      }
      if (0 != (HTuple(hv_CurrentColor!=HTuple("")).TupleAnd(hv_CurrentColor!=HTuple("auto"))))
      {
        //Valid color?
        try
        {
          SetColor(hv_WindowHandle, hv_CurrentColor);
        }
        // catch (Exception) 
        catch (HalconCpp::HException &HDevExpDefaultException)
        {
          HDevExpDefaultException.ToHTuple(&hv_Exception);
          hv_Exception = ((("Wrong value of control parameter Color["+(hv_Index%(hv_Color.TupleLength())))+"] == '")+hv_CurrentColor)+"' (must be a valid color string)";
          throw HalconCpp::HException(hv_Exception);
        }
      }
      else
      {
        SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
      }
      //Finally display text
      hv_RowI = hv_R1+(hv_MaxHeight*hv_Index);
      SetTposition(hv_WindowHandle, hv_RowI, hv_C1);
      WriteString(hv_WindowHandle, HTuple(hv_StringI[hv_Index]));
    }
    }
  }
  }
  //Reset changed window settings
  SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
  SetPart(hv_WindowHandle, hv_RowI1Part, hv_ColumnI1Part, hv_RowI2Part, hv_ColumnI2Part);
  return;
}

// Chapter: Graphics / Text
// Short Description: Set font independent of OS 
void set_display_font (HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold, 
    HTuple hv_Slant)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_OS, hv_BufferWindowHandle, hv_Ascent;
  HTuple  hv_Descent, hv_Width, hv_Height, hv_Scale, hv_Exception;
  HTuple  hv_SubFamily, hv_Fonts, hv_SystemFonts, hv_Guess;
  HTuple  hv_I, hv_Index, hv_AllowedFontSizes, hv_Distances;
  HTuple  hv_Indices, hv_FontSelRegexp, hv_FontsCourier;

  //This procedure sets the text font of the current window with
  //the specified attributes.
  //It is assumed that following fonts are installed on the system:
  //Windows: Courier New, Arial Times New Roman
  //Mac OS X: CourierNewPS, Arial, TimesNewRomanPS
  //Linux: courier, helvetica, times
  //Because fonts are displayed smaller on Linux than on Windows,
  //a scaling factor of 1.25 is used the get comparable results.
  //For Linux, only a limited number of font sizes is supported,
  //to get comparable results, it is recommended to use one of the
  //following sizes: 9, 11, 14, 16, 20, 27
  //(which will be mapped internally on Linux systems to 11, 14, 17, 20, 25, 34)
  //
  //Input parameters:
  //WindowHandle: The graphics window for which the font will be set
  //Size: The font size. If Size=-1, the default of 16 is used.
  //Bold: If set to 'true', a bold font is used
  //Slant: If set to 'true', a slanted font is used
  //
  GetSystem("operating_system", &hv_OS);
  // dev_get_preferences(...); only in hdevelop
  // dev_set_preferences(...); only in hdevelop
  if (0 != (HTuple(hv_Size==HTuple()).TupleOr(hv_Size==-1)))
  {
    hv_Size = 16;
  }
  if (0 != ((hv_OS.TupleSubstr(0,2))==HTuple("Win")))
  {
    //Set font on Windows systems
    try
    {
      //Check, if font scaling is switched on
      OpenWindow(0, 0, 256, 256, 0, "buffer", "", &hv_BufferWindowHandle);
      SetFont(hv_BufferWindowHandle, "-Consolas-16-*-0-*-*-1-");
      GetStringExtents(hv_BufferWindowHandle, "test_string", &hv_Ascent, &hv_Descent, 
          &hv_Width, &hv_Height);
      //Expected width is 110
      hv_Scale = 110.0/hv_Width;
      hv_Size = (hv_Size*hv_Scale).TupleInt();
      CloseWindow(hv_BufferWindowHandle);
    }
    // catch (Exception) 
    catch (HalconCpp::HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      //throw (Exception)
    }
    if (0 != (HTuple(hv_Font==HTuple("Courier")).TupleOr(hv_Font==HTuple("courier"))))
    {
      hv_Font = "Courier New";
    }
    else if (0 != (hv_Font==HTuple("mono")))
    {
      hv_Font = "Consolas";
    }
    else if (0 != (hv_Font==HTuple("sans")))
    {
      hv_Font = "Arial";
    }
    else if (0 != (hv_Font==HTuple("serif")))
    {
      hv_Font = "Times New Roman";
    }
    if (0 != (hv_Bold==HTuple("true")))
    {
      hv_Bold = 1;
    }
    else if (0 != (hv_Bold==HTuple("false")))
    {
      hv_Bold = 0;
    }
    else
    {
      hv_Exception = "Wrong value of control parameter Bold";
      throw HalconCpp::HException(hv_Exception);
    }
    if (0 != (hv_Slant==HTuple("true")))
    {
      hv_Slant = 1;
    }
    else if (0 != (hv_Slant==HTuple("false")))
    {
      hv_Slant = 0;
    }
    else
    {
      hv_Exception = "Wrong value of control parameter Slant";
      throw HalconCpp::HException(hv_Exception);
    }
    try
    {
      SetFont(hv_WindowHandle, ((((((("-"+hv_Font)+"-")+hv_Size)+"-*-")+hv_Slant)+"-*-*-")+hv_Bold)+"-");
    }
    // catch (Exception) 
    catch (HalconCpp::HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      //throw (Exception)
    }
  }
  else if (0 != ((hv_OS.TupleSubstr(0,2))==HTuple("Dar")))
  {
    //Set font on Mac OS X systems. Since OS X does not have a strict naming
    //scheme for font attributes, we use tables to determine the correct font
    //name.
    hv_SubFamily = 0;
    if (0 != (hv_Slant==HTuple("true")))
    {
      hv_SubFamily = hv_SubFamily|1;
    }
    else if (0 != (hv_Slant!=HTuple("false")))
    {
      hv_Exception = "Wrong value of control parameter Slant";
      throw HalconCpp::HException(hv_Exception);
    }
    if (0 != (hv_Bold==HTuple("true")))
    {
      hv_SubFamily = hv_SubFamily|2;
    }
    else if (0 != (hv_Bold!=HTuple("false")))
    {
      hv_Exception = "Wrong value of control parameter Bold";
      throw HalconCpp::HException(hv_Exception);
    }
    if (0 != (hv_Font==HTuple("mono")))
    {
      hv_Fonts.Clear();
      hv_Fonts[0] = "Menlo-Regular";
      hv_Fonts[1] = "Menlo-Italic";
      hv_Fonts[2] = "Menlo-Bold";
      hv_Fonts[3] = "Menlo-BoldItalic";
    }
    else if (0 != (HTuple(hv_Font==HTuple("Courier")).TupleOr(hv_Font==HTuple("courier"))))
    {
      hv_Fonts.Clear();
      hv_Fonts[0] = "CourierNewPSMT";
      hv_Fonts[1] = "CourierNewPS-ItalicMT";
      hv_Fonts[2] = "CourierNewPS-BoldMT";
      hv_Fonts[3] = "CourierNewPS-BoldItalicMT";
    }
    else if (0 != (hv_Font==HTuple("sans")))
    {
      hv_Fonts.Clear();
      hv_Fonts[0] = "ArialMT";
      hv_Fonts[1] = "Arial-ItalicMT";
      hv_Fonts[2] = "Arial-BoldMT";
      hv_Fonts[3] = "Arial-BoldItalicMT";
    }
    else if (0 != (hv_Font==HTuple("serif")))
    {
      hv_Fonts.Clear();
      hv_Fonts[0] = "TimesNewRomanPSMT";
      hv_Fonts[1] = "TimesNewRomanPS-ItalicMT";
      hv_Fonts[2] = "TimesNewRomanPS-BoldMT";
      hv_Fonts[3] = "TimesNewRomanPS-BoldItalicMT";
    }
    else
    {
      //Attempt to figure out which of the fonts installed on the system
      //the user could have meant.
      QueryFont(hv_WindowHandle, &hv_SystemFonts);
      hv_Fonts.Clear();
      hv_Fonts.Append(hv_Font);
      hv_Fonts.Append(hv_Font);
      hv_Fonts.Append(hv_Font);
      hv_Fonts.Append(hv_Font);
      hv_Guess.Clear();
      hv_Guess.Append(hv_Font);
      hv_Guess.Append(hv_Font+"-Regular");
      hv_Guess.Append(hv_Font+"MT");
      {
      HTuple end_val100 = (hv_Guess.TupleLength())-1;
      HTuple step_val100 = 1;
      for (hv_I=0; hv_I.Continue(end_val100, step_val100); hv_I += step_val100)
      {
        TupleFind(hv_SystemFonts, HTuple(hv_Guess[hv_I]), &hv_Index);
        if (0 != (hv_Index!=-1))
        {
          hv_Fonts[0] = HTuple(hv_Guess[hv_I]);
          break;
        }
      }
      }
      //Guess name of slanted font
      hv_Guess.Clear();
      hv_Guess.Append(hv_Font+"-Italic");
      hv_Guess.Append(hv_Font+"-ItalicMT");
      hv_Guess.Append(hv_Font+"-Oblique");
      {
      HTuple end_val109 = (hv_Guess.TupleLength())-1;
      HTuple step_val109 = 1;
      for (hv_I=0; hv_I.Continue(end_val109, step_val109); hv_I += step_val109)
      {
        TupleFind(hv_SystemFonts, HTuple(hv_Guess[hv_I]), &hv_Index);
        if (0 != (hv_Index!=-1))
        {
          hv_Fonts[1] = HTuple(hv_Guess[hv_I]);
          break;
        }
      }
      }
      //Guess name of bold font
      hv_Guess.Clear();
      hv_Guess.Append(hv_Font+"-Bold");
      hv_Guess.Append(hv_Font+"-BoldMT");
      {
      HTuple end_val118 = (hv_Guess.TupleLength())-1;
      HTuple step_val118 = 1;
      for (hv_I=0; hv_I.Continue(end_val118, step_val118); hv_I += step_val118)
      {
        TupleFind(hv_SystemFonts, HTuple(hv_Guess[hv_I]), &hv_Index);
        if (0 != (hv_Index!=-1))
        {
          hv_Fonts[2] = HTuple(hv_Guess[hv_I]);
          break;
        }
      }
      }
      //Guess name of bold slanted font
      hv_Guess.Clear();
      hv_Guess.Append(hv_Font+"-BoldItalic");
      hv_Guess.Append(hv_Font+"-BoldItalicMT");
      hv_Guess.Append(hv_Font+"-BoldOblique");
      {
      HTuple end_val127 = (hv_Guess.TupleLength())-1;
      HTuple step_val127 = 1;
      for (hv_I=0; hv_I.Continue(end_val127, step_val127); hv_I += step_val127)
      {
        TupleFind(hv_SystemFonts, HTuple(hv_Guess[hv_I]), &hv_Index);
        if (0 != (hv_Index!=-1))
        {
          hv_Fonts[3] = HTuple(hv_Guess[hv_I]);
          break;
        }
      }
      }
    }
    hv_Font = HTuple(hv_Fonts[hv_SubFamily]);
    try
    {
      SetFont(hv_WindowHandle, (hv_Font+"-")+hv_Size);
    }
    // catch (Exception) 
    catch (HalconCpp::HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      //throw (Exception)
    }
  }
  else
  {
    //Set font for UNIX systems
    hv_Size = hv_Size*1.25;
    hv_AllowedFontSizes.Clear();
    hv_AllowedFontSizes[0] = 11;
    hv_AllowedFontSizes[1] = 14;
    hv_AllowedFontSizes[2] = 17;
    hv_AllowedFontSizes[3] = 20;
    hv_AllowedFontSizes[4] = 25;
    hv_AllowedFontSizes[5] = 34;
    if (0 != ((hv_AllowedFontSizes.TupleFind(hv_Size))==-1))
    {
      hv_Distances = (hv_AllowedFontSizes-hv_Size).TupleAbs();
      TupleSortIndex(hv_Distances, &hv_Indices);
      hv_Size = HTuple(hv_AllowedFontSizes[HTuple(hv_Indices[0])]);
    }
    if (0 != (HTuple(hv_Font==HTuple("mono")).TupleOr(hv_Font==HTuple("Courier"))))
    {
      hv_Font = "courier";
    }
    else if (0 != (hv_Font==HTuple("sans")))
    {
      hv_Font = "helvetica";
    }
    else if (0 != (hv_Font==HTuple("serif")))
    {
      hv_Font = "times";
    }
    if (0 != (hv_Bold==HTuple("true")))
    {
      hv_Bold = "bold";
    }
    else if (0 != (hv_Bold==HTuple("false")))
    {
      hv_Bold = "medium";
    }
    else
    {
      hv_Exception = "Wrong value of control parameter Bold";
      throw HalconCpp::HException(hv_Exception);
    }
    if (0 != (hv_Slant==HTuple("true")))
    {
      if (0 != (hv_Font==HTuple("times")))
      {
        hv_Slant = "i";
      }
      else
      {
        hv_Slant = "o";
      }
    }
    else if (0 != (hv_Slant==HTuple("false")))
    {
      hv_Slant = "r";
    }
    else
    {
      hv_Exception = "Wrong value of control parameter Slant";
      throw HalconCpp::HException(hv_Exception);
    }
    try
    {
      SetFont(hv_WindowHandle, ((((((("-adobe-"+hv_Font)+"-")+hv_Bold)+"-")+hv_Slant)+"-normal-*-")+hv_Size)+"-*-*-*-*-*-*-*");
    }
    // catch (Exception) 
    catch (HalconCpp::HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      if (0 != (HTuple((hv_OS.TupleSubstr(0,4))==HTuple("Linux")).TupleAnd(hv_Font==HTuple("courier"))))
      {
        QueryFont(hv_WindowHandle, &hv_Fonts);
        hv_FontSelRegexp = (("^-[^-]*-[^-]*[Cc]ourier[^-]*-"+hv_Bold)+"-")+hv_Slant;
        hv_FontsCourier = (hv_Fonts.TupleRegexpSelect(hv_FontSelRegexp)).TupleRegexpMatch(hv_FontSelRegexp);
        if (0 != ((hv_FontsCourier.TupleLength())==0))
        {
          hv_Exception = "Wrong font name";
          //throw (Exception)
        }
        else
        {
          try
          {
            SetFont(hv_WindowHandle, ((HTuple(hv_FontsCourier[0])+"-normal-*-")+hv_Size)+"-*-*-*-*-*-*-*");
          }
          // catch (Exception) 
          catch (HalconCpp::HException &HDevExpDefaultException)
          {
            HDevExpDefaultException.ToHTuple(&hv_Exception);
            //throw (Exception)
          }
        }
      }
      //throw (Exception)
    }
  }
  // dev_set_preferences(...); only in hdevelop
  return;
}

#ifndef NO_EXPORT_MAIN
// Main procedure 
void action()
{

  // Local iconic variables
  HObject  ho_ImageFabrik, ho_ImagePatras, ho_Image;
  HObjectVector  hvec_ImageVector(1), hvec_ImageVectorSobel(1);

  // Local control variables
  HTuple  hv_WindowHandle, hv_Message, hv_Tuple1;
  HTuple  hv_Tuple2, hv_Tuple, hv_I, hv_Length, hv_Tuple3;
  HTuple  hv_TupleFromVector5, hv_TupleFromVector2dim;
  HTupleVector  hvec_Vector1(1), hvec_Vector2(1);
  HTupleVector  hvec_Vector3(1), hvec_Vector2dim(2), hvec_Vector3dim(3);
  HTupleVector  hvec_Vector4(1), hvec_Vector5(1), hvec_Vector3dimCopy(3);
  HTupleVector  hvec_Vector1FromTuple3(1), hvec_Vector2FromTuple3(1);

  //In addition to the two well established variable types in HDevelop,
  //tuples and iconic objects, there is a third variable type: the vector.
  //A vector is a container that can hold an arbitrary number of elements,
  //all of which must have the exact same variable type (i.e., tuple,
  //iconic object, or vector). Regarding memory management and the like,
  //vectors in HDevelop are implemented the same way as vectors in the
  //C++ Standard Template Library.
  //Nomenclature: A vector of tuples (or of iconic objects) is called
  //one dimensional, a vector of vectors of tuples two dimensional, and so on.
  //(Hence, tuples or iconic objects have a dimension of zero).
  //
  if (HDevWindowStack::IsOpen())
    CloseWindow(HDevWindowStack::Pop());
  dev_update_off();
  SetWindowAttr("background_color","black");
  OpenWindow(0,0,512,512,0,"","",&hv_WindowHandle);
  HDevWindowStack::Push(hv_WindowHandle);
  set_display_font(hv_WindowHandle, 16, "mono", "true", "false");
  hv_Message = "This program gives an overview of";
  hv_Message[1] = "vector variables and how to use them.";
  hv_Message[2] = "Please";
  hv_Message[3] = HTuple(" - step through the program,");
  hv_Message[4] = HTuple(" - read the comments,");
  hv_Message[5] = " - and check the variable view.";
  disp_message(hv_WindowHandle, hv_Message, "window", 12, 12, "black", "true");
  // stop(...); only in hdevelop
  //
  //Construction of vectors:
  //
  //The easiest way to write down a vector is to put a (comma separated)
  //list of its elements between curly brackets.
  hvec_Vector1 = ((HTupleVector(1).Insert(0,HTupleVector(HTuple(1)))).Insert(1,HTupleVector(HTuple(2))));
  //Note, that {[1,2]} is NOT the same as {1,2}:
  hvec_Vector2 = (HTupleVector(1).Insert(0,HTupleVector((HTuple(1).Append(2)))));
  //Vector elements may be constant (expressions) as well as variables.
  hv_Tuple1.Clear();
  hv_Tuple1[0] = 5;
  hv_Tuple1[1] = 6;
  hv_Tuple2 = "eight";
  hvec_Vector3 = ((HTupleVector(1).Insert(0,HTupleVector(hv_Tuple1))).Insert(1,HTupleVector(hv_Tuple2)));
  //Vectors can also contain iconic objects:
  ReadImage(&ho_ImageFabrik, "fabrik");
  ReadImage(&ho_ImagePatras, "patras");
  hvec_ImageVector = ((HObjectVector(1).Insert(0,HObjectVector(ho_ImageFabrik))).Insert(1,HObjectVector(ho_ImagePatras)));
  //It is possible to use multidimensional vectors:
  hvec_Vector2dim = ((HTupleVector(2).Insert(0,((HTupleVector(1).Insert(0,HTupleVector(HTuple(1)))).Insert(1,HTupleVector(HTuple(2)))))).Insert(1,hvec_Vector3));
  hvec_Vector3dim = ((HTupleVector(3).Insert(0,hvec_Vector2dim)).Insert(1,((HTupleVector(2).Insert(0,hvec_Vector1)).Insert(1,hvec_Vector2))));
  //Note, that all elements of a vector must have the same type.
  //E.g., the following does not work:
  //InvalidExpression := {Vector1,Vector2dim}
  //
  //Element access:
  //Vector elements are accessed via the .at() expression
  hv_Tuple = hvec_Vector3[0].T();
  ho_Image = hvec_ImageVector[1].O();
  //.at() expressions may as well serve as l-values.
  //E.g., they can be used as output parameters.
  hvec_Vector4[0] = HTupleVector(hv_Tuple);
  //Writing to a non existing vector element is allowed. If necessary,
  //the vector is automatically filled with empty elements.
  hvec_Vector5[4] = HTupleVector(hv_Tuple);
  //Vector elements can be used as parameters for operators or procedures
  //(as long as they have the required type).
  {
  HTuple end_val58 = hvec_ImageVector.Length()-1;
  HTuple step_val58 = 1;
  for (hv_I=0; hv_I.Continue(end_val58, step_val58); hv_I += step_val58)
  {
    {
    HObject ExpTmpOutVar_0;
    SobelAmp(hvec_ImageVector[hv_I].O(), &ExpTmpOutVar_0, "sum_abs", 3);
    hvec_ImageVectorSobel[hv_I].O() = ExpTmpOutVar_0;
    }
  }
  }
  //
  //It is also possible to combine several .at() expressions to access the
  //subelements of multidimensional vectors.
  hvec_Vector3dim[1][0] = ((HTupleVector(1).Insert(0,HTupleVector(HTuple(5)))).Insert(1,HTupleVector(HTuple(6))));
  //Even the following is possible (though not exactly recommended).
  hvec_Vector3dim[0][1][1].T()[0] = 8;
  //
  //Vector length:
  //The number of elements of a vector can be accessed with .length()
  hv_Length = hvec_Vector1.Length();
  //
  //Vector manipulation:
  //.clear() clears a vector, i.e., removes all its elements:
  hvec_Vector3dimCopy = hvec_Vector3dim;
  hvec_Vector3dimCopy.Clear();
  //Note, that the cleared vectors still keep their variable type. The
  //following assignments cause a type conflict and, therefore, are invalid!
  //Vector3dimCopy.at(0) := 1
  //Vector3dimCopy.at(0) := {1}
  //.remove() removes an element with given index from a vector.
  hvec_Vector5.Remove(0);
  //
  //.insert() inserts at the given position one vector element into a vector.
  hvec_Vector5.Insert(1,HTupleVector(HTuple(2)));
  //Again, the vector gets filled up with empty elements where necessary.
  hvec_Vector5.Insert(8,HTupleVector(HTuple(10)));
  //
  //Vector conversion:
  //Convert a tuple into a one-dimensional vector
  hv_Tuple3.Clear();
  hv_Tuple3[0] = 1;
  hv_Tuple3[1] = 2;
  hv_Tuple3[2] = 3;
  hv_Tuple3[3] = 4;
  hv_Tuple3[4] = 5;
  hv_Tuple3[5] = 6;
  hv_Tuple3[6] = 7;
  hv_Tuple3[7] = 8;
  hvec_Vector1FromTuple3 = HTupleVector(hv_Tuple3, 1);
  hvec_Vector2FromTuple3 = HTupleVector(hv_Tuple3, 2);
  //convert_vector_to_tuple concatenates all tuples that are contained in the
  //input vector of arbitrary dimension and stores them in the single
  //output tuple.
  hv_TupleFromVector5 = hvec_Vector5.ConvertVectorToTuple();
  hv_TupleFromVector2dim = hvec_Vector2dim.ConvertVectorToTuple();
  //
  //The variable view can be used to inspect vector variables
  // dev_inspect_ctrl(...); only in hdevelop
}


#ifndef NO_EXPORT_APP_MAIN

#ifdef __APPLE__
// On OS X systems, we must have a CFRunLoop running on the main thread in
// order for the HALCON graphics operators to work correctly, and run the
// action function in a separate thread. A CFRunLoopTimer is used to make sure
// the action function is not called before the CFRunLoop is running.
HTuple      gStartMutex;
H_pthread_t gActionThread;

static void timer_callback(CFRunLoopTimerRef timer, void *info)
{
  UnlockMutex(gStartMutex);
}

static Herror apple_action(void **parameters)
{
  LockMutex(gStartMutex);
  action();
  CFRunLoopStop(CFRunLoopGetMain());
  return H_MSG_OK;
}

static int apple_main(int argc, char *argv[])
{
  Herror                error;
  CFRunLoopTimerRef     Timer;
  CFRunLoopTimerContext TimerContext = { 0, 0, 0, 0, 0 };

  CreateMutex("type","sleep",&gStartMutex);
  LockMutex(gStartMutex);

  error = HpThreadHandleAlloc(&gActionThread);
  if (H_MSG_OK != error)
  {
    fprintf(stderr,"HpThreadHandleAlloc failed: %d\n", error);
    exit(1);
  }

  error = HpThreadCreate(gActionThread,0,apple_action);
  if (H_MSG_OK != error)
  {
    fprintf(stderr,"HpThreadCreate failed: %d\n", error);
    exit(1);
  }

  Timer = CFRunLoopTimerCreate(kCFAllocatorDefault,
                               CFAbsoluteTimeGetCurrent(),0,0,0,
                               timer_callback,&TimerContext);
  if (!Timer)
  {
    fprintf(stderr,"CFRunLoopTimerCreate failed\n");
    exit(1);
  }
  CFRunLoopAddTimer(CFRunLoopGetCurrent(),Timer,kCFRunLoopCommonModes);
  CFRunLoopRun();
  CFRunLoopRemoveTimer(CFRunLoopGetCurrent(),Timer,kCFRunLoopCommonModes);
  CFRelease(Timer);

  error = HpThreadHandleFree(gActionThread);
  if (H_MSG_OK != error)
  {
    fprintf(stderr,"HpThreadHandleFree failed: %d\n", error);
    exit(1);
  }

  ClearMutex(gStartMutex);
  return 0;
}
#endif

int main(int argc, char *argv[])
{
  // Default settings used in HDevelop (can be omitted) 
  int ret=0;
  SetSystem("width", 512);
  SetSystem("height", 512);

#if defined(_WIN32)
  SetSystem("use_window_thread", "true");
#elif defined(__linux__)
  XInitThreads();
#endif

#ifndef __APPLE__
  action();
#else
  ret = apple_main(argc,argv);
#endif
  return ret;
}

#endif


#endif


