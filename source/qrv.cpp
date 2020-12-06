// =============================================================================
//  qrv.cpp
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
  \file     qrv.cpp
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2020/11/23
  \brief
*/

// Includes --------------------------------------------------------------------
#include "qrv.h"
#include "OpenDialog.h"

// -----------------------------------------------------------------------------
// qrvWindow
// -----------------------------------------------------------------------------
qrvWindow::qrvWindow(QWidget *parent)
  : QMainWindow(parent)
{
  ibc::qt::ImageView  *imageView = new ibc::qt::ImageView();
  mScrollArea = new ibc::qt::ImageScrollArea(imageView);

  createTestPattern(&mImageData,
                    0,
                    ibc::image::ImageType::PIXEL_TYPE_MONO,
                    ibc::image::ImageType::DATA_TYPE_8BIT,
                    4096, 2048,
                    ibc::image::ColorMap::CMIndex_GrayScale,
                    1,
                    1.0, 0.0);
  mImageData.markAsImageModified();
  imageView->setImageDataPtr(&mImageData);

  mUI.setupUi(this);
  setCentralWidget(mScrollArea);

  connect(mUI.actionZoom_In, SIGNAL(triggered()), imageView, SLOT(slot_zoomIn()));
  connect(mUI.actionZoom_Out, SIGNAL(triggered()), imageView, SLOT(slot_zoomOut()));
}

// -----------------------------------------------------------------------------
// on_actionOpen_triggered
// -----------------------------------------------------------------------------
void qrvWindow::on_actionOpen_triggered(void)
{
/*  QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open RAW file"),
    "",
    tr("RAW File (*.raw);;All Files (*)"));*/

  OpenDialog dialog(this);
  if (dialog.exec() == 0)
    return;

}

// -----------------------------------------------------------------------------
// on_actionQuit_triggered
// -----------------------------------------------------------------------------
void qrvWindow::on_actionQuit_triggered(void)
{
  close();
}

// -----------------------------------------------------------------------------
// createTestPattern
// -----------------------------------------------------------------------------
void qrvWindow::createTestPattern(
    ibc::qt::ImageData *inImageData,
    int inPattern,
    ibc::image::ImageType::PixelType inPixelType,
    ibc::image::ImageType::DataType inDataType,
    int inWidth, int inHeight,
    ibc::image::ColorMap::ColorMapIndex inColorMapIndex,
    int inColorMapMultiNum,
    double inGain, double inOffsset)
{
  // Create ImageData here
  ibc::image::ImageType   imageType(inPixelType,
                                    ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                                    inDataType);
  ibc::image::ImageFormat imageFormat(imageType, inWidth, inHeight);
  inImageData->allocateImageBuffer(imageFormat);
  inImageData->mActiveConverter->setColorMapIndex(inColorMapIndex, inColorMapMultiNum);
  inImageData->mActiveConverter->setGain(inGain);
  inImageData->mActiveConverter->setOffset(inOffsset);

  fillTestPattern(inImageData, inPattern);
}

// -----------------------------------------------------------------------------
// fillTestPattern
// -----------------------------------------------------------------------------
void qrvWindow::fillTestPattern(ibc::qt::ImageData *inImageData, int inPattern)
{
  if (inImageData->getImageType().mPixelType == ibc::image::ImageType::PIXEL_TYPE_MONO &&
      inImageData->getImageType().mDataType  == ibc::image::ImageType::DATA_TYPE_8BIT)
  {
    unsigned char *bufPtr = (unsigned char *)inImageData->getImageBufferPtr();
    for (int y = 0; y < inImageData->getHeight(); y++)
      for (int x = 0; x < inImageData->getWidth(); x++)
      {
        switch (inPattern)
        {
          case 0:
            *bufPtr = (unsigned char)(x ^ y);
            break;
          case 1:
            *bufPtr = (unsigned char)(x & 0xFF);
            break;
          case 2:
            *bufPtr = (unsigned char)(y & 0xFF);
            break;
          default:
          //case 3:
            *bufPtr = (unsigned char)((x+y) & 0xFF);
            break;
        }
        bufPtr++;
      }
    return;
  }
  if (inImageData->getImageType().mPixelType == ibc::image::ImageType::PIXEL_TYPE_MONO &&
      inImageData->getImageType().mDataType  == ibc::image::ImageType::DATA_TYPE_16BIT)
  {
    unsigned short *bufPtr = (unsigned short *)inImageData->getImageBufferPtr();
    for (int y = 0; y < inImageData->getHeight(); y++)
      for (int x = 0; x < inImageData->getWidth(); x++)
      {
        switch (inPattern)
        {
          case 0:
            *bufPtr = (unsigned short)(x ^ y);
            break;
          case 1:
            *bufPtr = (unsigned short)(x & 0xFFFF);
            break;
          case 2:
            *bufPtr = (unsigned short)(y & 0xFFFF);
            break;
          case 3:
            *bufPtr = (unsigned short)((x+y) & 0xFFFF);
            break;
        }
        bufPtr++;
      }
    return;
  }
}
