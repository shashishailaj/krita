/*
 * This file is part of the KDE project
 *
 * Copyright (c) 2004 Cyrille Berger <cberger@cberger.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "klocale.h"

#include "kis_convolution_filter.h"

#include "kis_painter.h"
#include "kis_convolution_painter.h"

KisConvolutionFilter::KisConvolutionFilter(const KisID& id, KisView * view) :
	KisFilter( id, view )
{

}

void KisConvolutionFilter::process(KisPaintDeviceSP src,
				   KisPaintDeviceSP dst,
				   KisFilterConfiguration* configuration,
				   const QRect& rect)
{
	KisConvolutionPainter painter( dst );
	KisMatrix3x3* amatrixes = ((KisConvolutionConfiguration*)configuration)->matrixes();
	painter.applyMatrix(amatrixes, src, rect.x(), rect.y(), rect.width(), rect.height());
}

KisConvolutionConstFilter::~KisConvolutionConstFilter()
{
// XXX: Memory leak? Causes bug: http://bugs.kde.org/show_bug.cgi?id=95079
// 	if (m_matrixes) {
// 		kdDebug() << "Matrices exist!\n";
// 		delete m_matrixes;
// 	}
}

KisFilterConfiguration* KisConvolutionConstFilter::configuration(KisFilterConfigurationWidget*)
{
	return new KisConvolutionConfiguration( m_matrixes );
}

