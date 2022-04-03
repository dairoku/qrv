// =============================================================================
//  qrv.cpp
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

  // Initial format (for opendialog)
  mImageFormat.mType.set(ibc::image::ImageType::PIXEL_TYPE_MONO,
                         ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                         ibc::image::ImageType::DATA_TYPE_8BIT);
  mImageFormat.set(512, 512);

  mUI.setupUi(this);
  setCentralWidget(mScrollArea);

  connect(mUI.actionZoom_In, SIGNAL(triggered()), mImageView, SLOT(slot_zoomIn()));
  connect(mUI.actionZoom_Out, SIGNAL(triggered()), mImageView, SLOT(slot_zoomOut()));
}

// -----------------------------------------------------------------------------
// doRawFileOpenDialog
// -----------------------------------------------------------------------------
bool qrvWindow::doRawFileOpenDialog(void)
{
  QString fileName = QFileDialog::getOpenFileName(
    this,
    tr("Open RAW file"),
    "",
    tr("RAW File (*.raw);;All Files (*)"));
  if (fileName.isEmpty())
    return false;

  OpenDialog dialog(this);
  dialog.setImageFormat(mImageFormat);
  dialog.adjustSize();
  if (dialog.exec() == 0)
    return false;
  mImageFormat = dialog.getImageFormat();
  mImageFormat.dump();

  return openFile(fileName, mImageFormat);
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
  if ((size_t )fileStream.readRawData((char *)dataBuf, (int )fileSize) != fileSize)
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
// testPattern
// -----------------------------------------------------------------------------
bool  qrvWindow::testPattern(
      int inPattern,
      const ibc::image::ImageFormat &inImageFormat,
      ibc::image::ColorMap::ColorMapIndex inColorMapIndex,
      int inColorMapMultiNum,
      double inGain, double inOffsset)
{
  ibc::image::utils::TestPattern::allocateBufferAndFill(
                    &mImageData,
                    inPattern,
                    inImageFormat.mType.mPixelType,
                    inImageFormat.mType.mDataType,
                    inImageFormat.mWidth, inImageFormat.mHeight,
                    inColorMapIndex,
                    inColorMapMultiNum,
                    inGain, inOffsset);
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
  dialog.adjustSize();
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
