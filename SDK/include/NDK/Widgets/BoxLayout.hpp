// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Development Kit"
// For conditions of distribution and use, see copyright notice in Prerequisites.hpp

#pragma once

#ifndef NDK_WIDGETS_BOXLAYOUT_HPP
#define NDK_WIDGETS_BOXLAYOUT_HPP

#include <NDK/Prerequisites.hpp>
#include <NDK/Widgets/Enums.hpp>
#include <NDK/Widgets/LayoutWidget.hpp>

namespace Ndk
{
	class NDK_API BoxLayout : public LayoutWidget
	{
		public:
			inline BoxLayout(BoxLayoutOrientation orientation, BaseWidget* parent = nullptr);
			BoxLayout(const BoxLayout&) = delete;
			BoxLayout(BoxLayout&&) = default;
			~BoxLayout() = default;

			BoxLayout& operator=(const BoxLayout&) = delete;
			BoxLayout& operator=(BoxLayout&&) = default;

		private:
			void Layout() override;

			BoxLayoutOrientation m_orientation;
	};
}

#include <NDK/Widgets/BoxLayout.inl>

#endif // NDK_WIDGETS_BUTTONWIDGET_HPP
