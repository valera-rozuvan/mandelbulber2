/**
 * Mandelbulber v2, a 3D fractal generator       ,=#MKNmMMKmmßMNWy,
 *                                             ,B" ]L,,p%%%,,,§;, "K
 * Copyright (C) 2014-16 Krzysztof Marczak     §R-==%w["'~5]m%=L.=~5N
 *                                        ,=mm=§M ]=4 yJKA"/-Nsaj  "Bw,==,,
 * This file is part of Mandelbulber.    §R.r= jw",M  Km .mM  FW ",§=ß., ,TN
 *                                     ,4R =%["w[N=7]J '"5=],""]]M,w,-; T=]M
 * Mandelbulber is free software:     §R.ß~-Q/M=,=5"v"]=Qf,'§"M= =,M.§ Rz]M"Kw
 * you can redistribute it and/or     §w "xDY.J ' -"m=====WeC=\ ""%""y=%"]"" §
 * modify it under the terms of the    "§M=M =D=4"N #"%==A%p M§ M6  R' #"=~.4M
 * GNU General Public License as        §W =, ][T"]C  §  § '§ e===~ U  !§[Z ]N
 * published by the                    4M",,Jm=,"=e~  §  §  j]]""N  BmM"py=ßM
 * Free Software Foundation,          ]§ T,M=& 'YmMMpM9MMM%=w=,,=MT]M m§;'§,
 * either version 3 of the License,    TWw [.j"5=~N[=§%=%W,T ]R,"=="Y[LFT ]N
 * or (at your option)                   TW=,-#"%=;[  =Q:["V""  ],,M.m == ]N
 * any later version.                      J§"mr"] ,=,," =="""J]= M"M"]==ß"
 *                                          §= "=C=4 §"eM "=B:m|4"]#F,§~
 * Mandelbulber is distributed in            "9w=,,]w em%wJ '"~" ,=,,ß"
 * the hope that it will be useful,                 . "K=  ,=RMMMßM"""
 * but WITHOUT ANY WARRANTY;                            .'''
 * without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with Mandelbulber. If not, see <http://www.gnu.org/licenses/>.
 *
 * ###########################################################################
 *
 * Authors: Krzysztof Marczak (buddhi1980@gmail.com), Sebastian Jennen
 *
 * Functions for flight animation.
 *
 * cFlightAnimation contains all functionality for capturing, editing
 * and rendering of a flight path. The class holds a cAnimationFrames to store
 * the parameters of the frames and a table to display the frames in a widget.
 * The class keeps track of current velocity and acceleration and exposes
 * slots to change the camera acceleration.
 */

#ifndef MANDELBULBER2_SRC_ANIMATION_FLIGHT_HPP_
#define MANDELBULBER2_SRC_ANIMATION_FLIGHT_HPP_

#include "animation_frames.hpp"
#include "cimage.hpp"
#include "error_message.hpp"
#include "interface.hpp"

class cFlightAnimation : public QObject
{
	Q_OBJECT

public:
	enum enumSpeedMode
	{
		speedRelative,
		speedConstant
	};

	cFlightAnimation(cInterface *_interface, cAnimationFrames *_frames, cImage *_image,
		QWidget *_imageWidget, cParameterContainer *_params, cFractalContainer *_fractal,
		QObject *parent = 0);
	void RecordFlight(bool continueRecording);
	bool RenderFlight(bool *stopRequest);
	void RenderFrame(int index);
	void RefreshTable();
	QString GetParameterName(int rowNumber);
	void DeleteFramesFrom(int index);
	void DeleteFramesTo(int index);
	void UpdateThumbnailFromImage(int index);
	void InterpolateForward(int row, int column);
	QString GetFlightFilename(int index);
	void UpdateLimitsForFrameRange(void);

public slots:
	bool slotRenderFlight();

private slots:
	void slotRecordFlight();
	void slotContinueRecording();
	void slotFlightChangeSpeed(double amount);
	void slotFlightStrafe(CVector2<double> _strafe);
	void slotFlightYawAndPitch(CVector2<double> _yawAndPitch);
	void slotFlightRotation(double direction);
	void slotOrthogonalStrafe(bool _orthogonalStrafe);
	void slotSelectAnimFlightImageDir();
	void slotTableCellChanged(int row, int column);
	void slotDeleteAllImages();
	void slotShowAnimation();
	void slotRecordPause();
	void slotRefreshTable();
	void slotExportFlightToKeyframes();
	void slotMovedSliderFirstFrame(int value);
	void slotMovedSliderLastFrame(int value);
	void slotCellDoubleClicked(int row, int column);

private:
	void PrepareTable();
	void CreateRowsInTable();
	int AddVariableToTable(
		const cAnimationFrames::sParameterDescription &parameterDescription, int index);
	int AddColumn(const cAnimationFrames::sAnimationFrame &frame);
	cInterface *mainInterface;
	Ui::RenderWindow *ui;
	cAnimationFrames *frames;
	cImage *image;
	QWidget *imageWidget;
	cParameterContainer *params;
	cFractalContainer *fractalParams;
	QStringList tableRowNames;
	QVector<int> parameterRows; // position of parameter in table
	QVector<int> rowParameter;	// index of parameter in row
	MyTableWidgetAnim *table;
	CVector2<double> strafe;
	CVector2<double> yawAndPitch;
	double rotationDirection;
	bool orthogonalStrafe;
	double linearSpeedSp;
	// QList<cThumbnailWidget*> thumbnailWidgets;
	bool recordPause;

signals:
	void updateProgressAndStatus(const QString &text, const QString &progressText, double progress,
		cProgressText::enumProgressType progressType = cProgressText::progress_IMAGE);
	void updateStatistics(cStatistics statistics);
	void updateProgressHide(
		cProgressText::enumProgressType progressType = cProgressText::progress_ANIMATION);
	void QuestionMessage(const QString &questionTitle, const QString &questionText,
		QMessageBox::StandardButtons buttons, QMessageBox::StandardButton *reply);
	void showErrorMessage(
		QString text, cErrorMessage::enumMessageType messageType, QWidget *parent = NULL);
};

extern cFlightAnimation *gFlightAnimation;

#endif /* MANDELBULBER2_SRC_ANIMATION_FLIGHT_HPP_ */
