// =============================================================================
//  main.cpp
//
//  MIT License
//
//  Written in 2020 by Dairoku Sekiguchi (sekiguchi at acm dot org)
//
//  To the extent possible under law, the author(s) have dedicated all copyright
//  and related and neighboring rights to this software to the public domain worldwide.
//  This software is distributed without any warranty.
//
//  You should have received a copy of the CC0 Public Domain Dedication along with
//  this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
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
    parser.showHelp(0);
    //The above code will exit the program

  qrvWindow win;
  if (!args.empty() || parser.isSet("debug"))
  {
    if (!parser.isSet("width") || !parser.isSet("height"))
      parser.showHelp(-1);
      //The above code will exit the program

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
      if (!win.testPattern(
              (int )parser.value("debug").toUInt(),
              imageFormat,
              colorMapIndex,
              (int )parser.value("multimap").toUInt(),
              parser.value("gain").toDouble(),
              parser.value("offset").toDouble()))
        return -1;
    }
    else
    {
      if (!win.openFile(args[0], imageFormat))
        return -1;
    }
  }
  else
  {
  if (!win.doRawFileOpenDialog())
    return -1;
  }

  win.show();
  return QApplication::exec();
}
