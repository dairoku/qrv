// =============================================================================
//  OpenDialog.h
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
  \file     OpenDialog.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2020/12/06
  \brief
*/

#pragma once

// Includes --------------------------------------------------------------------
#include <QDialog>
#include "ibc/image/image.h"
#include "ui_OpenDialog.h"

// -----------------------------------------------------------------------------
// qpcvWindow class
// -----------------------------------------------------------------------------
class OpenDialog : public QDialog
{
Q_OBJECT

public:
  // Constructors and Destructor -----------------------------------------------
  OpenDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
    : QDialog(parent, f)
  {
    initUI();
  }
  // ---------------------------------------------------------------------------
  // setImageFormat
  // ---------------------------------------------------------------------------
  void  setImageFormat(const ibc::image::ImageFormat &inImageFormat)
  {
    mImageFormat = inImageFormat;
    updateUI();
  }
  // ---------------------------------------------------------------------------
  // getImageFormat
  // ---------------------------------------------------------------------------
  ibc::image::ImageFormat  getImageFormat()
  {
    updateImageFormat();
    return mImageFormat;
  }

protected:
  // Member variables ----------------------------------------------------------
  ibc::image::ImageFormat   mImageFormat;
  Ui_OpenDialog mOpenDialogUI;

  // Member functions ----------------------------------------------------------
  void  initUI()
  {
    mOpenDialogUI.setupUi(this);
    //
    mOpenDialogUI.mPixelType_Combo->addItem("Mono",       QVariant(ibc::image::ImageType::PIXEL_TYPE_MONO));
    mOpenDialogUI.mPixelType_Combo->addItem("RGB",        QVariant(ibc::image::ImageType::PIXEL_TYPE_RGB));
    mOpenDialogUI.mPixelType_Combo->addItem("BGR",        QVariant(ibc::image::ImageType::PIXEL_TYPE_BGR));
    mOpenDialogUI.mPixelType_Combo->addItem("BAYER_GBRG", QVariant(ibc::image::ImageType::PIXEL_TYPE_BAYER_GBRG));
    mOpenDialogUI.mPixelType_Combo->addItem("BAYER_GRBG", QVariant(ibc::image::ImageType::PIXEL_TYPE_BAYER_GRBG));
    mOpenDialogUI.mPixelType_Combo->addItem("BAYER_BGGR", QVariant(ibc::image::ImageType::PIXEL_TYPE_BAYER_BGGR));
    mOpenDialogUI.mPixelType_Combo->addItem("BAYER_RGGB", QVariant(ibc::image::ImageType::PIXEL_TYPE_BAYER_RGGB));
    //
    mOpenDialogUI.mBufferType_Combo->addItem("Aligned",   QVariant(ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED));
    mOpenDialogUI.mBufferType_Combo->addItem("Packed",    QVariant(ibc::image::ImageType::BUFFER_TYPE_PIXEL_PACKED));

    mOpenDialogUI.mDataType_Combo->addItem("8bit",        QVariant(ibc::image::ImageType::DATA_TYPE_8BIT));
    mOpenDialogUI.mDataType_Combo->addItem("16bit",       QVariant(ibc::image::ImageType::DATA_TYPE_16BIT));
    //
    //mOpenDialogUI.mChTotalNum_Combo->setEnabled(false);
    //mOpenDialogUI.mChIndex0_SpinBox->setEnabled(false);
    //mOpenDialogUI.mChIndex1_SpinBox->setEnabled(false);
    //mOpenDialogUI.mChIndex2_SpinBox->setEnabled(false);
    //
    //mOpenDialogUI.mPlannerFormat_Check->setEnabled(false);
    //
    mOpenDialogUI.mLineSize_Check->setEnabled(false);
    mOpenDialogUI.mLineSize_SpinBox->setEnabled(false);
    //
    mOpenDialogUI.mPlaneSize_Check->setEnabled(false);
    mOpenDialogUI.mPlaneSize_SpinBox->setEnabled(false);

    connect(mOpenDialogUI.mWidth_SpinBox,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            [=](int i)
            {
              UNUSED(i);
            });
    connect(mOpenDialogUI.buttonBox->button(QDialogButtonBox::Ok),
            static_cast<void(QPushButton::*)(bool)>(&QPushButton::clicked),
            this,
            [=](bool checked)
            {
              UNUSED(checked);
              updateImageFormat();
            });
  }

  void  updateUI()
  {
    mOpenDialogUI.mPixelType_Combo->setCurrentIndex(
      mOpenDialogUI.mPixelType_Combo->findData(
        mImageFormat.mType.mPixelType));

    mOpenDialogUI.mBufferType_Combo->setCurrentIndex(
      mOpenDialogUI.mBufferType_Combo->findData(
        mImageFormat.mType.mBufferType));

    mOpenDialogUI.mDataType_Combo->setCurrentIndex(
      mOpenDialogUI.mDataType_Combo->findData(
        mImageFormat.mType.mDataType));

    mOpenDialogUI.mWidth_SpinBox->setValue(mImageFormat.mWidth);
    mOpenDialogUI.mHeight_SpinBox->setValue(mImageFormat.mHeight);
  }

  void  updateImageFormat()
  {
    ibc::image::ImageType::PixelType  pixelType;
    pixelType = (ibc::image::ImageType::PixelType )mOpenDialogUI.mPixelType_Combo->currentData().value<int>();
    mImageFormat.mType.setPixelType(pixelType);

    ibc::image::ImageType::BufferType  bufferType;
    bufferType = (ibc::image::ImageType::BufferType )mOpenDialogUI.mBufferType_Combo->currentData().value<int>();
    mImageFormat.mType.setBufferType(bufferType);

    ibc::image::ImageType::DataType  dataType;
    dataType = (ibc::image::ImageType::DataType )mOpenDialogUI.mDataType_Combo->currentData().value<int>();
    mImageFormat.mType.setDataType(dataType);

    mImageFormat.set(
      mOpenDialogUI.mWidth_SpinBox->value(),  // width
      mOpenDialogUI.mHeight_SpinBox->value()  // height
    );
  }

};
