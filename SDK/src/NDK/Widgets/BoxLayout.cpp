// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Development Kit"
// For conditions of distribution and use, see copyright notice in Prerequisites.hpp

#include <NDK/Widgets/BoxLayout.hpp>

namespace Ndk
{
	void BoxLayout::Layout()
	{
		BaseWidget::Layout();

		std::size_t childCount = 0;
		Nz::Vector2f size = Nz::Vector2f::Zero();
		ForEachChild([&](BaseWidget* child)
		{
			size += child->GetSize();
			childCount++;
		});

		Nz::Vector2f widgetSize = size / float(childCount);

		float cursor = 0.f;
		switch (m_orientation)
		{
			case BoxLayoutOrientation_Horizontal:
			{
				ForEachChild([&](BaseWidget* child)
				{
					child->SetPosition(0.f, cursor);
					child->SetSize(widgetSize);

					cursor += widgetSize.x;
				});
				break;
			}

			case BoxLayoutOrientation_Vertical:
			{
				ForEachChild([&](BaseWidget* child)
				{
					child->SetPosition(cursor, 0.f);
					child->SetSize(widgetSize);

					cursor += widgetSize.y;
				});
				break;
			}

			default:
				break;
		}
	}
}
