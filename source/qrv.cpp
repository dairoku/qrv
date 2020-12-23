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
#include <QFile>
#include <QFileInfo>
#include "ibc/image/utils/test_pattern.h"
#include "qrv.h"
#include "OpenDialog.h"

// -----------------------------------------------------------------------------
// qrvWindow
// -----------------------------------------------------------------------------
qrvWindow::qrvWindow(QWidget *parent)
  : QMainWindow(parent)
{
  mImageView = new ibc::qt::ImageView();
  mScrollArea = new ibc::qt::ImageScrollArea(mImageView);

  mImageFormat.mType.set(ibc::image::ImageType::PIXEL_TYPE_MONO,
                         ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                         ibc::image::ImageType::DATA_TYPE_8BIT);
  mImageFormat.set(512, 512);

  ibc::image::utils::TestPattern::prepareTestPattern(
                    &mImageData,
                    0,
                    ibc::image::ImageType::PIXEL_TYPE_MONO,
                    ibc::image::ImageType::DATA_TYPE_8BIT,
                    4096, 4096,
                    ibc::image::ColorMap::CMIndex_GrayScale,
                    1,
                    1.0, 0.0);
  mImageData.markAsImageModified();
  mImageView->setImageDataPtr(&mImageData);

  mUI.setupUi(this);
  setCentralWidget(mScrollArea);

  connect(mUI.actionZoom_In, SIGNAL(triggered()), mImageView, SLOT(slot_zoomIn()));
  connect(mUI.actionZoom_Out, SIGNAL(triggered()), mImageView, SLOT(slot_zoomOut()));
}

// -----------------------------------------------------------------------------
// openFile
// -----------------------------------------------------------------------------
bool  qrvWindow::openFile(
        const QString &fileName,
        const ibc::image::ImageFormat &inImageFormat)
{
  mImageData.allocateImageBuffer(inImageFormat);

  QFileInfo fileInfo(fileName);
  if (fileInfo.isReadable() == false)
  {
    printf("Can't open file\n");
    return false;
  }

  size_t fileSize = fileInfo.size();
  if (fileSize == 0)
  {
    printf("File size is zero \n");
    return false;
  }
  unsigned char *dataBuf = new unsigned char[fileSize];
  if (dataBuf == NULL)
  {
    printf("Can't allocate data buffer: \n");
    return false;
  }

  QFile file(fileName);
  if (file.open(QIODevice::ReadOnly) == false)
  {
    printf("Can't open file \n");
    return false;
  }
  QDataStream fileStream(&file);
  if (fileStream.readRawData((char *)dataBuf, fileSize) != (int )fileSize)
  {
    printf("Can't read file \n");
    return false;
  }
  file.close();

  size_t  copySize = mImageData.getImageBufferSize();
  if (copySize > fileSize)
    copySize = fileSize;
  memcpy(mImageData.getImageBufferPtr(), dataBuf, copySize);

  //mImageData.mActiveConverter->setColorMapIndex(colorMapIndex, multiMap);
  //mImageData.mActiveConverter->setGain(mGain);
  //mImageData.mActiveConverter->setOffset(mOffset);
  mImageData.markAsImageModified();
  mImageView->setImageDataPtr(&mImageData);

  return true;
}

// -----------------------------------------------------------------------------
// on_actionOpen_triggered
// -----------------------------------------------------------------------------
void qrvWindow::on_actionOpen_triggered(void)
{
  QString fileName = QFileDialog::getOpenFileName(
    this,
    tr("Open RAW file"),
    "",
    tr("RAW File (*.raw);;All Files (*)"));
  if (fileName.isEmpty())
    return;

  OpenDialog dialog(this);
  dialog.setImageFormat(mImageFormat);
  if (dialog.exec() == 0)
    return;
  mImageFormat = dialog.getImageFormat();
  mImageFormat.dump();

  openFile(fileName, mImageFormat);
}

// -----------------------------------------------------------------------------
// on_actionQuit_triggered
// -----------------------------------------------------------------------------
void qrvWindow::on_actionQuit_triggered(void)
{
  close();
}

