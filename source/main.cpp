// =============================================================================
//  main.cpp
//
//  MIT License
//
//  Copyright (c) 2020 Dairoku Sekiguchi
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
// =============================================================================
/*!
  \file     main.cpp
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2020/11/23
  \brief
*/

#include <QtWidgets/QApplication>
#include "qrv.h"

// -----------------------------------------------------------------------------
// main
// -----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
//#ifdef _WIN32
#if 0
  FILE  *std_stream, *err_stream;
  ::AllocConsole();
  freopen_s(&std_stream, "CONOUT$", "a", stdout);
  freopen_s(&err_stream, "CONOUT$", "a", stderr);
#endif

  QApplication app(argc, argv);
  QApplication::setApplicationName("qrv");
  QApplication::setApplicationVersion("v0.0.1");

  QCommandLineParser  parser;
  parser.setApplicationDescription("Simple raw file viewer");
  //parser.addHelpOption(); (<- We can't use this. Since there is '-h' option)
  parser.addVersionOption();
  parser.addPositionalArgument("input", QCoreApplication::translate("main", "Input file"));
  parser.addOptions({
    {
      {"w", "width", "x"},
      QCoreApplication::translate("main", "Width of the input image"),
      QCoreApplication::translate("main", "width")
    },
    {
      {"h", "height", "y"},
      QCoreApplication::translate("main", "Height of the input image"),
      QCoreApplication::translate("main", "height")
    },
    {
      {"p", "pixel"},
      QCoreApplication::translate("main", "Pixel type (default: MONO)"),
      QCoreApplication::translate("main", "pixel"),
      QCoreApplication::translate("main", "MONO")
    },
    {
      {"b", "buffer"},
      QCoreApplication::translate("main", "Buffer type (default: ALIGNED)"),
      QCoreApplication::translate("main", "buffer"),
      QCoreApplication::translate("main", "ALIGNED")
    },
    {
      {"d", "data"},
      QCoreApplication::translate("main", "Data type (default: 8BIT)"),
      QCoreApplication::translate("main", "data"),
      QCoreApplication::translate("main", "8BIT")
    },
    {
      {"c", "colormap"},
      QCoreApplication::translate("main", "Colormap (default: GrayScale)"),
      QCoreApplication::translate("main", "colormap"),
      QCoreApplication::translate("main", "GrayScale")
    },
    {
      {"m", "multimap"},
      QCoreApplication::translate("main", "Color map multimap num (default: 1)"),
      QCoreApplication::translate("main", "multimap"),
      QCoreApplication::translate("main", "1")
    },
    {
      {"g", "gain"},
      QCoreApplication::translate("main", "Color map gain num (default: 1.0)"),
      QCoreApplication::translate("main", "gain"),
      QCoreApplication::translate("main", "1.0")
    },
    {
      {"o", "offset"},
      QCoreApplication::translate("main", "Value Offset (default: 0.0)"),
      QCoreApplication::translate("main", "offset"),
      QCoreApplication::translate("main", "0.0")
    },
    {
      "debug",
      QCoreApplication::translate("main", "Debug option (0: xor, 1: H gradation)"),
      QCoreApplication::translate("main", "debug")
    },
    {
      "help",
      QCoreApplication::translate("main", "Show help")
    }
  });
  parser.process(app);
  const QStringList args = parser.positionalArguments();
  if (parser.isSet("help"))
  {
    parser.showHelp(0);
    return 0;
  }

  qrvWindow win;
  if (args.size() != 0 || parser.isSet("debug"))
  {
    if (parser.isSet("width") == false || parser.isSet("height") == false)
    {
      parser.showHelp(-1);
      return -1;
    }
    ibc::image::ImageFormat imageFormat;
    imageFormat.mType.set(
      ibc::image::ImageType::stringToPixelType(parser.value("pixel").toStdString().c_str()),
      ibc::image::ImageType::stringToBufferlType(parser.value("buffer").toStdString().c_str()),
      ibc::image::ImageType::stringToDataType(parser.value("data").toStdString().c_str()));
    imageFormat.set(
      parser.value("width").toUInt(),
      parser.value("height").toUInt());
    if (parser.isSet("debug"))
    {
      ibc::image::ColorMap::ColorMapIndex colorMapIndex = ibc::image::ColorMap::CMIndex_GrayScale;
      colorMapIndex =
        ibc::image::ColorMap::stringToColorMapIndex(
          parser.value("colormap").toStdString().c_str(), colorMapIndex);
      if (win.testPattern(
            parser.value("debug").toUInt(),
            imageFormat,
            colorMapIndex,
            parser.value("multimap").toUInt(),
            parser.value("gain").toDouble(),
            parser.value("offset").toDouble()) == false)
        return -1;
    }
    else
    {
      if (win.openFile(args[0], imageFormat) == false)
        return -1;
    }
  }
  else
  {
  if (win.doRawFileOpenDialog() == false)
    return -1;
  }

  win.show();
  return app.exec();
}
