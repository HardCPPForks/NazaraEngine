// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Development Kit"
// For conditions of distribution and use, see copyright notice in Prerequisites.hpp

#include <NDK/Widgets/BoxLayout.hpp>

namespace Ndk
{
	inline BoxLayout::BoxLayout(BoxLayoutOrientation orientation, BaseWidget* parent) :
	LayoutWidget(parent),
	m_orientation(orientation)
	{
	}
}
