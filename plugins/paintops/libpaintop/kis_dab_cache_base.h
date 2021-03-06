/*
 *  Copyright (c) 2012 Dmitry Kazakov <dimula73@gmail.com>
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
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __KIS_DAB_CACHE_BASE_H
#define __KIS_DAB_CACHE_BASE_H

#include "kritapaintop_export.h"
#include "kis_brush.h"

#include "KisDabCacheUtils.h"

class KisColorSource;
class KisPressureSharpnessOption;
class KisTextureProperties;
class KisPressureMirrorOption;
class KisPrecisionOption;
struct MirrorProperties;


/**
 * @brief The KisDabCacheBase class provides caching for dabs into the brush paintop
 *
 *  This class adds caching of the dabs to the paintop system of Krita.
 *  Such cache makes the execution of the benchmarks up to 2 times faster.
 *  Subjectively, the real painting becomes much faster, especially with
 *  huge brushes. Artists report up to 20% speed gain while painting.
 *
 *  Of course, such caching makes the painting a bit less precise: we need
 *  to tolerate subpixel differences to allow the cache to work. Sometimes
 *  small difference in the size of a dab can also be acceptable. That is
 *  why I introduced levels of precision. They are graded from 1 to 5: from
 *  the fastest and less precise to the slowest, but with the best quality.
 *  You can see the slider in the paintop settings dialog. The ToolTip text
 *  explains which features of the brush are sacrificed on each precision
 *  level.
 *
 *  The texturing and mirroring problems are solved.
 */
class PAINTOP_EXPORT KisDabCacheBase
{
public:
    KisDabCacheBase();
    ~KisDabCacheBase();

    void setMirrorPostprocessing(KisPressureMirrorOption *option);
    void setPrecisionOption(KisPrecisionOption *option);

    /**
     * Disables handling of the subPixelX and subPixelY values, this
     * is needed at least for the Color Smudge paint op, which reads
     * aligned areas from image, so additional offsets generated by
     * the subpixel precision should be avoided
     */
    void disableSubpixelPrecision();

    /**
     * Return true if the dab needs postprocessing by special options
     * like 'texture' or 'sharpness'
     */
    bool needSeparateOriginal(KisTextureProperties *textureOption,
                              KisPressureSharpnessOption *sharpnessOption) const;

protected:
    /**
     * Fetches all the necessary information for dab generation and
     * tells if the caller must should reuse the preciously returned dab. *
     * Please note that KisDabCacheBase has an internal state, that keeps the
     * parameters of the previously generated (on a cache-miss) dab. This function
     * automatically updates this state when 'shouldUseCache == false'. Therefore, the
     * caller *must* generate the dab if and only if when 'shouldUseCache == false'.
     * Otherwise the internal state will become inconsistent.
     *
     * @param hasDabInCache shows if the caller has something in its cache
     * @param resources rendering resources available for this dab
     * @param request the request information
     * @param di (OUT) calculated dab generation information
     * @param shouldUseCache (OUT) shows whether the caller *must* use cache or not
     */
    void fetchDabGenerationInfo(bool hasDabInCache,
                                KisDabCacheUtils::DabRenderingResources *resources,
                                const KisDabCacheUtils::DabRequestInfo &request,
                                /* out */
                                KisDabCacheUtils::DabGenerationInfo *di,
                                bool *shouldUseCache);

private:
    struct SavedDabParameters;
    struct DabPosition;
private:
    inline SavedDabParameters getDabParameters(KisBrushSP brush, const KoColor& color,
                                               KisDabShape const&,
                                               const KisPaintInformation& info,
                                               double subPixelX, double subPixelY,
                                               qreal softnessFactor,
                                               MirrorProperties mirrorProperties);

    inline KisDabCacheBase::DabPosition
    calculateDabRect(KisBrushSP brush, const QPointF &cursorPoint,
                     KisDabShape,
                     const KisPaintInformation& info,
                     const MirrorProperties &mirrorProperties, KisPressureSharpnessOption *sharpnessOption);

private:
    struct Private;
    Private * const m_d;
};

#endif /* __KIS_DAB_CACHE_BASE_H */
