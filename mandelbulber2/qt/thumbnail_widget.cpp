/**
 * Mandelbulber v2, a 3D fractal generator       ,=#MKNmMMKmmßMNWy,
 *                                             ,B" ]L,,p%%%,,,§;, "K
 * Copyright (C) 2016 Krzysztof Marczak        §R-==%w["'~5]m%=L.=~5N
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
 * Authors: Krzysztof Marczak (buddhi1980@gmail.com)
 *
 * cThumbnailWidget - promoted QWidget for displaying of fractals in an auto-rendering thumbnail
 *
 * This class holds an cImage and fractal settings can be assigned with AssignParameters().
 * The class then asynchroniously renders the fractal as a thumbnail and displays it.
 * The fractal thumbnails can also be cached in filesystem for faster loading.
 * Signals for progress and render finish can be connected, see also usage in PreviewFileDialog.
 */

#include "thumbnail_widget.h"
#include "../src/global_data.hpp"
#include "../src/render_job.hpp"
#include "../src/settings.hpp"
#include <QImage>
#include <QPaintEvent>

#include "../src/common_math.h"
#include "../src/system.hpp"

cThumbnailWidget::cThumbnailWidget(int _width, int _height, int _oversample, QWidget *parent)
		: QWidget(parent)
{
	tWidth = _width;
	tHeight = _height;
	oversample = _oversample;
	image = new cImage(tWidth * oversample, tHeight * oversample);
	image->CreatePreview(1.0 / oversample, tWidth, tWidth, this);
	progressBar = NULL;
	setFixedWidth(tWidth);
	setFixedHeight(tHeight);
	stopRequest = false;
	isRendered = false;
	hasParameters = false;
	disableTimer = false;
	disableThumbnailCache = false;
	connect(this, SIGNAL(renderRequest()), this, SLOT(slotRender()));
	params = new cParameterContainer;
	fractal = new cFractalContainer;
	useOneCPUCore = false;

	timer = new QTimer(parent);
	timer->setSingleShot(true);
	connect(timer, SIGNAL(timeout()), this, SLOT(slotRandomRender()));

	lastRenderTime = 0.0;

	instanceIndex = instanceCount;
	instanceCount++;
	// qDebug() << "cThumbnailWidget constructed" << instanceCount;
}

cThumbnailWidget::~cThumbnailWidget()
{
	stopRequest = true;
	if (image)
	{
		// qDebug() << "cThumbnailWidget trying to delete" << instanceIndex;
		while (image->IsUsed())
		{
		}
		delete image;
		// qDebug() << "cThumbnailWidget image deleted" << instanceIndex;
	}
	if (params) delete params;
	if (fractal) delete fractal;

	instanceCount--;
	// qDebug() << "cThumbnailWidget destructed" << instanceIndex;
}

void cThumbnailWidget::paintEvent(QPaintEvent *event)
{
	event->accept();
	if (hasParameters)
	{
		if (!isRendered)
		{
			isRendered = true;
			timer->stop();
			emit renderRequest();
		}
	}
	image->RedrawInWidget(this);
}

void cThumbnailWidget::AssignParameters(
	const cParameterContainer &_params, const cFractalContainer &_fractal)
{
	*params = _params;
	*fractal = _fractal;
	params->Set("image_width", tWidth * oversample);
	params->Set("image_height", tHeight * oversample);
	cSettings tempSettings(cSettings::formatCondensedText);
	tempSettings.CreateText(params, fractal);
	oldHash = hash;
	hash = tempSettings.GetHashCode();

	if (hash != oldHash)
	{
		stopRequest = true;
		while (image->IsUsed())
		{
			// just wait and pray
		}

		emit settingsChanged();

		isRendered = false;
		hasParameters = true;

		QString thumbnailFileName = systemData.thumbnailDir + hash + QString(".png");
		if (QFileInfo::exists(thumbnailFileName) && !disableThumbnailCache)
		{
			stopRequest = true;
			isRendered = true;
			while (image->IsUsed())
			{
				// just wait and pray
			}

			QPixmap pixmap;
			pixmap.load(thumbnailFileName);
			pixmap = pixmap.scaled(tWidth, tHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			QImage qimage = pixmap.toImage();
			qimage = qimage.convertToFormat(QImage::Format_RGB888);
			sRGB8 *bitmap;
			bitmap = (sRGB8 *)(qimage.bits());
			int bwidth = qimage.width();
			int bheight = qimage.height();
			sRGB8 *previewPointer = (sRGB8 *)image->GetPreviewPrimaryPtr();
			sRGB8 *preview2Pointer = (sRGB8 *)image->GetPreviewPtr();
			memcpy(previewPointer, bitmap, sizeof(sRGB8) * bwidth * bheight);
			memcpy(preview2Pointer, bitmap, sizeof(sRGB8) * bwidth * bheight);
			emit thumbnailRendered();
		}
		else
		{
			if (!disableTimer)
			{
				// render thumbnail after random time. It forces rendering of widgets when they are not
				// visible. It makes rendering of widgets when they are idle.

				timer->start(Random(100000) * 10 + 1);
			}
		}
	}
}

void cThumbnailWidget::slotRender()
{

	stopRequest = true;
	while (image->IsUsed())
	{
		// just wait and pray
		Wait(100);
	}

	// random wait to not generate to many events at exactly the same time
	Wait(Random(100) + 50);
	stopRequest = false;

	cRenderJob *renderJob = new cRenderJob(params, fractal, image, &stopRequest, (QWidget *)this);
	connect(renderJob, SIGNAL(updateProgressAndStatus(const QString &, const QString &, double)),
		this, SIGNAL(updateProgressAndStatus(const QString &, const QString &, double)));

	renderingTimeTimer.start();
	renderJob->UseSizeFromImage(true);

	cRenderingConfiguration config;
	if (useOneCPUCore) config.DisableMultiThread();
	config.EnableIgnoreErros();

	renderJob->Init(cRenderJob::still, config);

	QThread *thread = new QThread;
	renderJob->moveToThread(thread);
	QObject::connect(thread, SIGNAL(started()), renderJob, SLOT(slotExecute()));

	while (renderJob->GetRunningJobCount() > systemData.numberOfThreads * 5)
	{
	}
	thread->setObjectName("ThumbnailWorker");
	thread->start();

	QObject::connect(renderJob, SIGNAL(finished()), renderJob, SLOT(deleteLater()));
	QObject::connect(renderJob, SIGNAL(finished()), thread, SLOT(quit()));
	QObject::connect(renderJob, SIGNAL(fullyRendered()), this, SLOT(slotFullyRendered()));
	QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
}

void cThumbnailWidget::slotFullyRendered()
{
	isRendered = true;
	if (!disableThumbnailCache)
	{
		QImage qImage((const uchar *)image->ConvertTo8bit(), image->GetWidth(), image->GetHeight(),
			image->GetWidth() * sizeof(sRGB8), QImage::Format_RGB888);
		QPixmap pixmap;
		pixmap.convertFromImage(qImage);

		QString thumbnailFileName = systemData.thumbnailDir + hash + QString(".png");
		pixmap.save(thumbnailFileName, "PNG");
	}
	lastRenderTime = renderingTimeTimer.nsecsElapsed() / 1e9;
	emit thumbnailRendered();
}

void cThumbnailWidget::slotRandomRender()
{
	if (cRenderJob::GetRunningJobCount() > systemData.numberOfThreads)
	{
		// if it's to busy, render later
		timer->start(Random(100000) * 10 + 1);
	}
	else
	{
		isRendered = true;
		slotRender();
	}
}

void cThumbnailWidget::slotSetMinimumSize(int width, int height)
{
	setMinimumSize(width, height);
}

int cThumbnailWidget::instanceCount = 0;
