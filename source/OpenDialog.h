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
    mOpenDialogUI.mDataType_Combo->addItem("Unsigned");
    mOpenDialogUI.mDataType_Combo->addItem("Signed");
    //
    mOpenDialogUI.mBitWidth_Combo->addItem("8");
    mOpenDialogUI.mBitWidth_Combo->addItem("16");
    //
    mOpenDialogUI.mPackedType_Combo->addItem("None");
    mOpenDialogUI.mPackedType_Combo->addItem("Packed");
    //
    mOpenDialogUI.mColorModel_Combo->addItem("Mono");
    mOpenDialogUI.mColorModel_Combo->addItem("RGB");
    //
    mOpenDialogUI.mChTotalNum_Combo->setEnabled(false);
    mOpenDialogUI.mChIndex0_SpinBox->setEnabled(false);
    mOpenDialogUI.mChIndex1_SpinBox->setEnabled(false);
    mOpenDialogUI.mChIndex2_SpinBox->setEnabled(false);
    //
    mOpenDialogUI.mPlannerFormat_Check->setEnabled(false);
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
              //printf("%d\n", i);
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
    if (mImageFormat.mType.isSigned() == false)
      mOpenDialogUI.mDataType_Combo->setCurrentIndex(0);
    else
      mOpenDialogUI.mDataType_Combo->setCurrentIndex(1);

    if (mImageFormat.mType.sizeOfData() == 1)
      mOpenDialogUI.mBitWidth_Combo->setCurrentIndex(0);
    else
      mOpenDialogUI.mBitWidth_Combo->setCurrentIndex(1);

    if (mImageFormat.mType.isPacked() == false)
      mOpenDialogUI.mPackedType_Combo->setCurrentIndex(0);
    else
      mOpenDialogUI.mPackedType_Combo->setCurrentIndex(1);

    if (mImageFormat.mType.mPixelType == ibc::image::ImageType::PIXEL_TYPE_MONO)
      mOpenDialogUI.mColorModel_Combo->setCurrentIndex(0);
    else
      mOpenDialogUI.mColorModel_Combo->setCurrentIndex(1);

    mOpenDialogUI.mWidth_SpinBox->setValue(mImageFormat.mWidth);
    mOpenDialogUI.mHeight_SpinBox->setValue(mImageFormat.mHeight);
  }

  void  updateImageFormat()
  {
    if (mOpenDialogUI.mBitWidth_Combo->currentIndex() == 0)
    {
      if (mOpenDialogUI.mDataType_Combo->currentIndex() == 0)
        mImageFormat.mType.mDataType = ibc::image::ImageType::DATA_TYPE_8BIT;
      else
        mImageFormat.mType.mDataType = ibc::image::ImageType::DATA_TYPE_8BIT_SIGNED;
    }
    else
    {
      if (mOpenDialogUI.mDataType_Combo->currentIndex() == 0)
        mImageFormat.mType.mDataType = ibc::image::ImageType::DATA_TYPE_16BIT;
      else
        mImageFormat.mType.mDataType = ibc::image::ImageType::DATA_TYPE_16BIT_SIGNED;
    }

    if (mOpenDialogUI.mPackedType_Combo->currentIndex() == 0)
      mImageFormat.mType.mBufferType = ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED;
    else
      mImageFormat.mType.mBufferType = ibc::image::ImageType::BUFFER_TYPE_PIXEL_PACKED;

    if (mOpenDialogUI.mColorModel_Combo->currentIndex() == 0)
      mImageFormat.mType.mPixelType = ibc::image::ImageType::PIXEL_TYPE_MONO;
    else
      mImageFormat.mType.mPixelType = ibc::image::ImageType::PIXEL_TYPE_RGB;

    mImageFormat.mWidth = mOpenDialogUI.mWidth_SpinBox->value();
    mImageFormat.mHeight = mOpenDialogUI.mHeight_SpinBox->value();
  }

};
