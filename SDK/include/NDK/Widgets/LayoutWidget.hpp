// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Development Kit"
// For conditions of distribution and use, see copyright notice in Prerequisites.hpp

#pragma once

#ifndef NDK_WIDGETS_LAYOUTWIDGET_HPP
#define NDK_WIDGETS_LAYOUTWIDGET_HPP

#include <NDK/Prerequisites.hpp>
#include <NDK/BaseWidget.hpp>

namespace Ndk
{
	class NDK_API LayoutWidget : public BaseWidget
	{
		public:
			using BaseWidget::BaseWidget;
			LayoutWidget(const LayoutWidget&) = delete;
			LayoutWidget(LayoutWidget&&) = default;
			~LayoutWidget() = default;

			void ResizeToContent() override;

			LayoutWidget& operator=(const LayoutWidget&) = delete;
			LayoutWidget& operator=(LayoutWidget&&) = default;

		private:
			virtual void Layout() override = 0;
	};
}

#include <NDK/Widgets/LayoutWidget.inl>

#endif // NDK_WIDGETS_LAYOUTWIDGET_HPP
