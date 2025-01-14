/*
 * Copyright (C) 2004, 2005, 2007 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2004, 2005, 2007 Rob Buis <buis@kde.org>
 * Copyright (C) 2009 Google, Inc.
 * Copyright (C) 2009 Apple Inc. All rights reserved.
 * Copyright (c) 2020, 2021, 2022 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#pragma once

#if ENABLE(LAYER_BASED_SVG_ENGINE)
#include "RenderSVGContainer.h"

namespace WebCore {

class SVGSVGElement;

class RenderSVGViewportContainer final : public RenderSVGContainer {
    WTF_MAKE_ISO_ALLOCATED(RenderSVGViewportContainer);
public:
    RenderSVGViewportContainer(SVGSVGElement&, RenderStyle&&);

    SVGSVGElement& svgSVGElement() const;

    // FIXME: [LBSE] Centralize implementation in RenderSVGContainer (follow-up commit)
    bool isLayoutSizeChanged() const { return false; }

private:
    bool isSVGViewportContainer() const final { return true; }
    ASCIILiteral renderName() const final { return "RenderSVGViewportContainer"_s; }

    void element() const = delete;

    FloatRect computeViewport() const;
    void applyTransform(TransformationMatrix&, const RenderStyle&, const FloatRect& boundingBox, OptionSet<RenderStyle::TransformOperationOption> = RenderStyle::allTransformOperations) const final;
    LayoutRect overflowClipRect(const LayoutPoint&, RenderFragmentContainer*, OverlayScrollbarSizeRelevancy, PaintPhase) const final { return { }; }
    void updateLayerTransform() final;

    AffineTransform m_supplementalLayerTransform;
    FloatRect m_viewport;
};

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_RENDER_OBJECT(RenderSVGViewportContainer, isSVGViewportContainer())

#endif // ENABLE(LAYER_BASED_SVG_ENGINE)
