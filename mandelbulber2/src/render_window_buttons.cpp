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
 * RenderWindow class - main program window
 *
 * This file contains implementation of the buttton callbacks in RenderWindow class.
 * See also header render_window.hpp and whole implementation of class
 * spread over render_window_*.cpp
 */

#include "render_window.hpp"
#include "common_math.h"
#include "initparameters.hpp"
#include "interface.hpp"

void RenderWindow::slotPressedButtonAutoFog()
{
	gMainInterface->AutoFog();
}

void RenderWindow::slotPressedButtonDeletePrimitive()
{
	QString buttonName = this->sender()->objectName();
	QString primitiveName = buttonName.mid(buttonName.indexOf('_') + 1);
	gMainInterface->DeletePrimitive(primitiveName);
}

void RenderWindow::slotPressedButtonDOFUpdate()
{
	gMainInterface->RefreshMainImage();
}

void RenderWindow::slotPressedButtonGetJuliaConstant()
{
	QList<QVariant> item;
	item.append((int)RenderedImage::clickGetJuliaConstant);
	int index = ui->comboBox_mouse_click_function->findData(item);
	ui->comboBox_mouse_click_function->setCurrentIndex(index);
	gMainInterface->renderedImage->setClickMode(item);
}

void RenderWindow::slotPressedButtonGetPoint()
{
	QList<QVariant> item;
	item.append((int)RenderedImage::clickGetPoint);
	int index = ui->comboBox_mouse_click_function->findData(item);
	ui->comboBox_mouse_click_function->setCurrentIndex(index);
	gMainInterface->renderedImage->setClickMode(item);
}

void RenderWindow::slotPressedButtonImageApply()
{
	gMainInterface->RefreshMainImage();
}

void RenderWindow::slotPressedButtonNewPrimitive()
{
	QString buttonName = this->sender()->objectName();
	QString primitiveName = buttonName.mid(buttonName.lastIndexOf('_') + 1);
	gMainInterface->NewPrimitive(primitiveName);
}

void RenderWindow::slotPressedButtonOptimizeForLQ()
{
	gMainInterface->OptimizeStepFactor(1.0);
}

void RenderWindow::slotPressedButtonOptimizeForMQ()
{
	gMainInterface->OptimizeStepFactor(0.1);
}
void RenderWindow::slotPressedButtonOptimizeForHQ()
{
	gMainInterface->OptimizeStepFactor(0.01);
}

void RenderWindow::slotPressedButtonResetFormula()
{
	QString comboName = this->sender()->objectName();
	int fractalNumber = comboName.right(1).toInt() - 1;
	gMainInterface->ResetFormula(fractalNumber);
}

void RenderWindow::slotPressedButtonResetView()
{
	gMainInterface->ResetView();
}

void RenderWindow::slotPressedButtonSetDOFByMouse()
{
	QList<QVariant> item;
	item.append((int)RenderedImage::clickDOFFocus);
	int index = ui->comboBox_mouse_click_function->findData(item);
	ui->comboBox_mouse_click_function->setCurrentIndex(index);
	gMainInterface->renderedImage->setClickMode(item);
}

void RenderWindow::slotPressedButtonSetFogByMouse()
{
	QList<QVariant> item;
	item.append((int)RenderedImage::clickFogVisibility);
	int index = ui->comboBox_mouse_click_function->findData(item);
	ui->comboBox_mouse_click_function->setCurrentIndex(index);
	gMainInterface->renderedImage->setClickMode(item);
}

void RenderWindow::slotPressedButtonSetLight1ByMouse()
{
	QList<QVariant> item;
	item.append((int)RenderedImage::clickPlaceLight);
	item.append(1); // light number
	int index = ui->comboBox_mouse_click_function->findData(item);
	ui->comboBox_mouse_click_function->setCurrentIndex(index);
	gMainInterface->renderedImage->setClickMode(item);
	double distance = gMainInterface->GetDistanceForPoint(gPar->Get<CVector3>("camera"));
	ui->logedit_aux_light_manual_placement_dist->setText(QString("%L1").arg(distance * 0.1));
}

void RenderWindow::slotPressedButtonSetLight2ByMouse()
{
	QList<QVariant> item;
	item.append((int)RenderedImage::clickPlaceLight);
	item.append(2); // light number
	int index = ui->comboBox_mouse_click_function->findData(item);
	ui->comboBox_mouse_click_function->setCurrentIndex(index);
	gMainInterface->renderedImage->setClickMode(item);
}

void RenderWindow::slotPressedButtonSetLight3ByMouse()
{
	QList<QVariant> item;
	item.append((int)RenderedImage::clickPlaceLight);
	item.append(3); // light number
	int index = ui->comboBox_mouse_click_function->findData(item);
	ui->comboBox_mouse_click_function->setCurrentIndex(index);
	gMainInterface->renderedImage->setClickMode(item);
}

void RenderWindow::slotPressedButtonSetLight4ByMouse()
{
	QList<QVariant> item;
	item.append((int)RenderedImage::clickPlaceLight);
	item.append(4); // light number
	int index = ui->comboBox_mouse_click_function->findData(item);
	ui->comboBox_mouse_click_function->setCurrentIndex(index);
	gMainInterface->renderedImage->setClickMode(item);
}

void RenderWindow::slotPressedButtonPlaceRandomLightsByMouse()
{
	QList<QVariant> item;
	item.append((int)RenderedImage::clickPlaceRandomLightCenter);
	int index = ui->comboBox_mouse_click_function->findData(item);
	ui->comboBox_mouse_click_function->setCurrentIndex(index);
	gMainInterface->renderedImage->setClickMode(item);
}

void RenderWindow::slotPressedButtonSetPositionPrimitive()
{
	QString buttonName = this->sender()->objectName();
	QString primitiveName = buttonName.mid(buttonName.indexOf('_') + 1);
	QStringList split = primitiveName.split('_');
	QList<QVariant> item;
	item.append((int)RenderedImage::clickPlacePrimitive);
	item.append((int)PrimitiveNameToEnum(split.at(1)));
	item.append(split.at(2).toInt());
	item.append(primitiveName); // light number
	int index = ui->comboBox_mouse_click_function->findData(item);
	ui->comboBox_mouse_click_function->setCurrentIndex(index);
	gMainInterface->renderedImage->setClickMode(item);
}

void RenderWindow::slotPressedButtonIFSDefaultsDodecahedron()
{
	int index = ui->tabWidget_fractals->currentIndex();
	SynchronizeInterfaceWindow(fractalWidgets[index], &gParFractal->at(index), qInterface::read);
	gMainInterface->IFSDefaultsDodecahedron(&gParFractal->at(index));
	SynchronizeInterfaceWindow(fractalWidgets[index], &gParFractal->at(index), qInterface::write);
}

void RenderWindow::slotPressedButtonIFSDefaultsIcosahedron()
{
	int index = ui->tabWidget_fractals->currentIndex();
	SynchronizeInterfaceWindow(fractalWidgets[index], &gParFractal->at(index), qInterface::read);
	gMainInterface->IFSDefaultsIcosahedron(&gParFractal->at(index));
	SynchronizeInterfaceWindow(fractalWidgets[index], &gParFractal->at(index), qInterface::write);
}

void RenderWindow::slotPressedButtonIFSDefaultsOctahedron()
{
	int index = ui->tabWidget_fractals->currentIndex();
	SynchronizeInterfaceWindow(fractalWidgets[index], &gParFractal->at(index), qInterface::read);
	gMainInterface->IFSDefaultsOctahedron(&gParFractal->at(index));
	SynchronizeInterfaceWindow(fractalWidgets[index], &gParFractal->at(index), qInterface::write);
}

void RenderWindow::slotPressedButtonIFSDefaultsMengerSponge()
{
	int index = ui->tabWidget_fractals->currentIndex();
	SynchronizeInterfaceWindow(fractalWidgets[index], &gParFractal->at(index), qInterface::read);
	gMainInterface->IFSDefaultsMengerSponge(&gParFractal->at(index));
	SynchronizeInterfaceWindow(fractalWidgets[index], &gParFractal->at(index), qInterface::write);
}

void RenderWindow::slotPressedButtonIFSDefaultsReset()
{
	int index = ui->tabWidget_fractals->currentIndex();
	SynchronizeInterfaceWindow(fractalWidgets[index], &gParFractal->at(index), qInterface::read);
	gMainInterface->IFSDefaultsReset(&gParFractal->at(index));
	SynchronizeInterfaceWindow(fractalWidgets[index], &gParFractal->at(index), qInterface::write);
}

void RenderWindow::slotPressedImagesizeIncrease()
{
	int width = ui->spinboxInt_image_width->value();
	int height = ui->spinboxInt_image_height->value();
	width *= 2;
	height *= 2;
	ui->spinboxInt_image_width->setValue(width);
	ui->spinboxInt_image_height->setValue(height);
}

void RenderWindow::slotPressedImagesizeDecrease()
{
	int width = ui->spinboxInt_image_width->value();
	int height = ui->spinboxInt_image_height->value();
	width /= 2;
	height /= 2;
	ui->spinboxInt_image_width->setValue(width);
	ui->spinboxInt_image_height->setValue(height);
}
