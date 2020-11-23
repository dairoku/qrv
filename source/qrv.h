// =============================================================================
//  qrv.h
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
  \file     qrv.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2020/11/23
  \brief
*/

#pragma once

// Includes --------------------------------------------------------------------
#include <QtWidgets/QMainWindow>
#include "ui_qrv.h"
#include "ibc/qt/image_data.h"
#include "ibc/qt/image_scroll_area.h"

// -----------------------------------------------------------------------------
// qpcvWindow class
// -----------------------------------------------------------------------------
class qrvWindow : public QMainWindow
{
Q_OBJECT

public:
  // Constructors and Destructor -----------------------------------------------
  qrvWindow(QWidget *parent = Q_NULLPTR);

protected:
  // Member variables ----------------------------------------------------------
  ibc::qt::ImageScrollArea *mScrollArea;
  ibc::qt::ImageData        mImageData;

  // Member functions ----------------------------------------------------------
  void createTestPattern(
          ibc::qt::ImageData *inImageData,
          int inPattern,
          ibc::image::ImageType::PixelType inPixelType,
          ibc::image::ImageType::DataType inDataType,
          int inWidth, int inHeight,
          ibc::image::ColorMap::ColorMapIndex inColorMapIndex,
          int inColorMapMultiNum,
          double inGain, double inOffsset);
  void fillTestPattern(ibc::qt::ImageData *inImageData, int inPattern);

private:
  // Member variables ----------------------------------------------------------
  Ui::qrvClass mUI;

private slots:
  void on_actionOpen_triggered(void);
  void on_actionQuit_triggered(void);
};
