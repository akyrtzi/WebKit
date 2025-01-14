/*
 * Copyright (C) 2022 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(LAYOUT_FORMATTING_CONTEXT)

#include "FlexFormattingConstraints.h"
#include "FlexFormattingState.h"
#include "FlexRect.h"
#include <wtf/Range.h>

namespace WebCore {
namespace Layout {

// This class implements the layout logic for flex formatting contexts.
// https://www.w3.org/TR/css-flexbox-1/
class FlexLayout {
public:
    FlexLayout(const ContainerBox& flexBox);

    struct LogicalFlexItem {
    public:
        LogicalFlexItem(LayoutSize marginBoxSize, LengthType widthType, LengthType heightType, IntrinsicWidthConstraints, const ContainerBox&);
        LogicalFlexItem() = default;

        LayoutUnit flexBasis() const { return m_marginBoxSize.value.width(); }

        LayoutUnit width() const { return std::min(maximumSize(), std::max(minimumSize(), m_marginBoxSize.value.width())); }
        LayoutUnit height() const { return m_marginBoxSize.value.height(); }

        bool isHeightAuto() const { return m_marginBoxSize.heightType == LengthType::Auto; }

        LayoutUnit minimumSize() const { return m_intrinsicWidthConstraints.minimum; }
        LayoutUnit maximumSize() const { return m_intrinsicWidthConstraints.maximum; }

        const RenderStyle& style() const { return m_layoutBox->style(); }
        const ContainerBox& layoutBox() const { return *m_layoutBox; }

    private:
        struct MarginBoxSize {
            LayoutSize value;
            LengthType widthType { LengthType::Auto };
            LengthType heightType { LengthType::Auto };
        };
        MarginBoxSize m_marginBoxSize { };
        IntrinsicWidthConstraints m_intrinsicWidthConstraints { };
        CheckedPtr<const ContainerBox> m_layoutBox;        
    };
    using LogicalFlexItems = Vector<LogicalFlexItem>;
    struct LogicalConstraints {
        std::optional<LayoutUnit> verticalSpace;
        struct HorizontalSpace {
            std::optional<LayoutUnit> available; // This is how much space there is for flexing in main axis direction.
            std::optional<LayoutUnit> minimum; // This is how much space is at least available for flexing in main axis direction.
        };
        HorizontalSpace horizontalSpace;
    };
    using LogicalFlexItemRects = Vector<FlexRect>;
    LogicalFlexItemRects layout(const LogicalConstraints&, const LogicalFlexItems&);

private:
    using LineRange = WTF::Range<size_t>;
    void computeLogicalWidthForFlexItems(const LogicalFlexItems&, const LineRange&, LayoutUnit availableSpace, LogicalFlexItemRects&);
    void computeLogicalWidthForStretchingFlexItems(const LogicalFlexItems&, const LineRange&, LayoutUnit availableSpace, LogicalFlexItemRects&);
    void computeLogicalWidthForShrinkingFlexItems(const LogicalFlexItems&, const LineRange&, LayoutUnit availableSpace, LogicalFlexItemRects&);
    void computeLogicalHeightForFlexItems(const LogicalFlexItems&, const LineRange&, LayoutUnit availableSpace, LogicalFlexItemRects&);
    void alignFlexItems(const LogicalFlexItems&, const LineRange&, VerticalConstraints, LogicalFlexItemRects&);
    void justifyFlexItems(const LogicalFlexItems&, const LineRange&, LayoutUnit availableSpace, LogicalFlexItemRects&);

    using WrappingPositions = Vector<size_t>;
    WrappingPositions computeWrappingPositions(const LogicalFlexItems&, LayoutUnit availableSpace) const;
    LayoutUnit computeAvailableLogicalHorizontalSpace(const LogicalFlexItems&, const LogicalConstraints&) const;

    using LineHeightList = Vector<LayoutUnit>;
    LineHeightList computeAvailableLogicalVerticalSpace(const LogicalFlexItems&, const WrappingPositions&, const LogicalConstraints&) const;

    const ContainerBox& flexBox() const { return m_flexBox; }
    const RenderStyle& flexBoxStyle() const { return flexBox().style(); }

    const ContainerBox& m_flexBox;
};

inline FlexLayout::LogicalFlexItem::LogicalFlexItem(LayoutSize marginBoxSize, LengthType widthType, LengthType heightType, IntrinsicWidthConstraints intrinsicWidthConstraints, const ContainerBox& layoutBox)
    : m_marginBoxSize({ marginBoxSize, widthType, heightType })
    , m_intrinsicWidthConstraints(intrinsicWidthConstraints)
    , m_layoutBox(layoutBox)
{
}

}
}

#endif
