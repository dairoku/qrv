// =============================================================================
//  qrv.h
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
#include "ibc/image/image.h"
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

  // Member functions ----------------------------------------------------------
  bool doRawFileOpenDialog();
  bool openFile(const QString &fileName, const ibc::image::ImageFormat &inImageFormat);
  bool testPattern(
      int inPattern,
      const ibc::image::ImageFormat &inImageFormat,
      ibc::image::ColorMap::ColorMapIndex inColorMapIndex = ibc::image::ColorMap::CMIndex_NOT_SPECIFIED,
      int inColorMapMultiNum = 1,
      double inGain = 1.0, double inOffsset = 0);

protected:
  // Member variables ----------------------------------------------------------
  ibc::qt::ImageView        *mImageView;
  ibc::qt::ImageScrollArea  *mScrollArea;
  ibc::qt::ImageData        mImageData;
  ibc::image::ImageFormat   mImageFormat;

private:
  // Member variables ----------------------------------------------------------
  Ui::qrvClass mUI;

private slots:
  void on_actionOpen_triggered(void);
  void on_actionQuit_triggered(void);
  //
};
