// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Graphics/ForwardRenderQueue.hpp>
#include <Nazara/Graphics/AbstractViewer.hpp>
#include <Nazara/Utility/VertexStruct.hpp>
#include <limits>
#include <Nazara/Graphics/Debug.hpp>

///TODO: Replace sinus/cosinus by a lookup table (which will lead to a speed up about 10x)

namespace Nz
{
	/*!
	* \ingroup graphics
	* \class Nz::ForwardRenderQueue
	* \brief Graphics class that represents the rendering queue for forward rendering
	*/

	/*!
	* \brief Adds multiple billboards to the queue
	*
	* \param renderOrder Order of rendering
	* \param material Material of the billboards
	* \param count Number of billboards
	* \param positionPtr Position of the billboards
	* \param sizePtr Sizes of the billboards
	* \param sinCosPtr Rotation of the billboards if null, Vector2f(0.f, 1.f) is used
	* \param colorPtr Color of the billboards if null, Color::White is used
	*
	* \remark Produces a NazaraAssert if material is invalid
	*/

	void ForwardRenderQueue::AddBillboards(int renderOrder, const Material* material, std::size_t billboardCount, const Recti& scissorRect, SparsePtr<const Vector3f> positionPtr, SparsePtr<const Vector2f> sizePtr, SparsePtr<const Vector2f> sinCosPtr, SparsePtr<const Color> colorPtr)
	{
		NazaraAssert(material, "Invalid material");

		Vector2f defaultSinCos(0.f, 1.f); // sin(0) = 0, cos(0) = 1

		if (!sinCosPtr)
			sinCosPtr.Reset(&defaultSinCos, 0); // The trick here is to put the stride to zero, which leads the pointer to be immobile

		if (!colorPtr)
			colorPtr.Reset(&Color::White, 0); // Same

		if (material->IsDepthSortingEnabled())
		{
			for (std::size_t i = 0; i < billboardCount; ++i)
			{
				depthSortedBillboards.Insert({
					renderOrder,
					material,
					scissorRect,
					{
						*colorPtr++,
						*positionPtr++,
						*sizePtr++,
						*sinCosPtr++
					}
				});
			}
		}
		else
		{
			std::size_t billboardIndex = m_billboards.size();
			m_billboards.resize(billboardIndex + billboardCount);
			BillboardData* data = &m_billboards[billboardIndex];

			for (std::size_t i = 0; i < billboardCount; ++i)
			{
				data->center = *positionPtr++;
				data->color  = *colorPtr++;
				data->sinCos = *sinCosPtr++;
				data->size   = *sizePtr++;
				data++;
			}

			billboards.Insert({
				renderOrder,
				material,
				scissorRect,
				billboardCount,
				billboardIndex
			});
		}
	}

	/*!
	* \brief Adds multiple billboards to the queue
	*
	* \param renderOrder Order of rendering
	* \param material Material of the billboards
	* \param count Number of billboards
	* \param positionPtr Position of the billboards
	* \param sizePtr Sizes of the billboards
	* \param sinCosPtr Rotation of the billboards if null, Vector2f(0.f, 1.f) is used
	* \param alphaPtr Alpha parameters of the billboards if null, 1.f is used
	*
	* \remark Produces a NazaraAssert if material is invalid
	*/

	void ForwardRenderQueue::AddBillboards(int renderOrder, const Material* material, std::size_t billboardCount, const Recti& scissorRect, SparsePtr<const Vector3f> positionPtr, SparsePtr<const Vector2f> sizePtr, SparsePtr<const Vector2f> sinCosPtr, SparsePtr<const float> alphaPtr)
	{
		NazaraAssert(material, "Invalid material");

		Vector2f defaultSinCos(0.f, 1.f); // sin(0) = 0, cos(0) = 1

		if (!sinCosPtr)
			sinCosPtr.Reset(&defaultSinCos, 0); // The trick here is to put the stride to zero, which leads the pointer to be immobile

		float defaultAlpha = 1.f;

		if (!alphaPtr)
			alphaPtr.Reset(&defaultAlpha, 0); // Same

		if (material->IsDepthSortingEnabled())
		{
			for (std::size_t i = 0; i < billboardCount; ++i)
			{
				depthSortedBillboards.Insert({
					renderOrder,
					material,
					scissorRect,
					{
						ComputeColor(*alphaPtr++),
						*positionPtr++,
						*sizePtr++,
						*sinCosPtr++
					}
				});
			}
		}
		else
		{
			std::size_t billboardIndex = m_billboards.size();
			m_billboards.resize(billboardIndex + billboardCount);
			BillboardData* data = &m_billboards[billboardIndex];

			for (std::size_t i = 0; i < billboardCount; ++i)
			{
				data->center = *positionPtr++;
				data->color  = ComputeColor(*alphaPtr++);
				data->sinCos = *sinCosPtr++;
				data->size   = *sizePtr++;
				data++;
			}

			billboards.Insert({
				renderOrder,
				material,
				scissorRect,
				billboardCount,
				billboardIndex
			});
		}
	}

	/*!
	* \brief Adds multiple billboards to the queue
	*
	* \param renderOrder Order of rendering
	* \param material Material of the billboards
	* \param count Number of billboards
	* \param positionPtr Position of the billboards
	* \param sizePtr Sizes of the billboards
	* \param anglePtr Rotation of the billboards if null, 0.f is used
	* \param colorPtr Color of the billboards if null, Color::White is used
	*
	* \remark Produces a NazaraAssert if material is invalid
	*/

	void ForwardRenderQueue::AddBillboards(int renderOrder, const Material* material, std::size_t billboardCount, const Recti& scissorRect, SparsePtr<const Vector3f> positionPtr, SparsePtr<const Vector2f> sizePtr, SparsePtr<const float> anglePtr, SparsePtr<const Color> colorPtr)
	{
		NazaraAssert(material, "Invalid material");

		float defaultRotation = 0.f;

		if (!anglePtr)
			anglePtr.Reset(&defaultRotation, 0); // The trick here is to put the stride to zero, which leads the pointer to be immobile

		if (!colorPtr)
			colorPtr.Reset(&Color::White, 0); // Same

		if (material->IsDepthSortingEnabled())
		{
			for (std::size_t i = 0; i < billboardCount; ++i)
			{
				depthSortedBillboards.Insert({
					renderOrder,
					material,
					scissorRect,
					{
						*colorPtr++,
						*positionPtr++,
						*sizePtr++,
						ComputeSinCos(*anglePtr++)
					}
				});
			}
		}
		else
		{
			std::size_t billboardIndex = m_billboards.size();
			m_billboards.resize(billboardIndex + billboardCount);
			BillboardData* data = &m_billboards[billboardIndex];

			for (std::size_t i = 0; i < billboardCount; ++i)
			{
				data->center = *positionPtr++;
				data->color  = *colorPtr++;
				data->sinCos = ComputeSinCos(*anglePtr++);
				data->size   = *sizePtr++;
				data++;
			}

			billboards.Insert({
				renderOrder,
				material,
				scissorRect,
				billboardCount,
				billboardIndex
			});
		}
	}

	/*!
	* \brief Adds multiple billboards to the queue
	*
	* \param renderOrder Order of rendering
	* \param material Material of the billboards
	* \param count Number of billboards
	* \param positionPtr Position of the billboards
	* \param sizePtr Sizes of the billboards
	* \param anglePtr Rotation of the billboards if null, 0.f is used
	* \param alphaPtr Alpha parameters of the billboards if null, 1.f is used
	*
	* \remark Produces a NazaraAssert if material is invalid
	*/

	void ForwardRenderQueue::AddBillboards(int renderOrder, const Material* material, std::size_t billboardCount, const Recti& scissorRect, SparsePtr<const Vector3f> positionPtr, SparsePtr<const Vector2f> sizePtr, SparsePtr<const float> anglePtr, SparsePtr<const float> alphaPtr)
	{
		NazaraAssert(material, "Invalid material");

		float defaultRotation = 0.f;

		if (!anglePtr)
			anglePtr.Reset(&defaultRotation, 0); // The trick here is to put the stride to zero, which leads the pointer to be immobile

		float defaultAlpha = 1.f;

		if (!alphaPtr)
			alphaPtr.Reset(&defaultAlpha, 0); // Same
		
		if (material->IsDepthSortingEnabled())
		{
			for (std::size_t i = 0; i < billboardCount; ++i)
			{
				depthSortedBillboards.Insert({
					renderOrder,
					material,
					scissorRect,
					{
						ComputeColor(*alphaPtr++),
						*positionPtr++,
						*sizePtr++,
						ComputeSinCos(*anglePtr++)
					}
				});
			}
		}
		else
		{
			std::size_t billboardIndex = m_billboards.size();
			m_billboards.resize(billboardIndex + billboardCount);
			BillboardData* data = &m_billboards[billboardIndex];

			for (std::size_t i = 0; i < billboardCount; ++i)
			{
				data->center = *positionPtr++;
				data->color  = ComputeColor(*alphaPtr++);
				data->sinCos = ComputeSinCos(*anglePtr++);
				data->size   = *sizePtr++;
				data++;
			}

			billboards.Insert({
				renderOrder,
				material,
				scissorRect,
				billboardCount,
				billboardIndex
			});
		}
	}

	/*!
	* \brief Adds multiple billboards to the queue
	*
	* \param renderOrder Order of rendering
	* \param material Material of the billboards
	* \param count Number of billboards
	* \param positionPtr Position of the billboards
	* \param sizePtr Size of the billboards
	* \param sinCosPtr Rotation of the billboards if null, Vector2f(0.f, 1.f) is used
	* \param colorPtr Color of the billboards if null, Color::White is used
	*
	* \remark Produces a NazaraAssert if material is invalid
	*/

	void ForwardRenderQueue::AddBillboards(int renderOrder, const Material* material, std::size_t billboardCount, const Recti& scissorRect, SparsePtr<const Vector3f> positionPtr, SparsePtr<const float> sizePtr, SparsePtr<const Vector2f> sinCosPtr, SparsePtr<const Color> colorPtr)
	{
		NazaraAssert(material, "Invalid material");

		Vector2f defaultSinCos(0.f, 1.f); // sin(0) = 0, cos(0) = 1

		if (!sinCosPtr)
			sinCosPtr.Reset(&defaultSinCos, 0); // The trick here is to put the stride to zero, which leads the pointer to be immobile

		if (!colorPtr)
			colorPtr.Reset(&Color::White, 0); // Same
		
		if (material->IsDepthSortingEnabled())
		{
			for (std::size_t i = 0; i < billboardCount; ++i)
			{
				depthSortedBillboards.Insert({
					renderOrder,
					material,
					scissorRect,
					{
						*colorPtr++,
						*positionPtr++,
						ComputeSize(*sizePtr++),
						*sinCosPtr++
					}
				});
			}
		}
		else
		{
			std::size_t billboardIndex = m_billboards.size();
			m_billboards.resize(billboardIndex + billboardCount);
			BillboardData* data = &m_billboards[billboardIndex];

			for (std::size_t i = 0; i < billboardCount; ++i)
			{
				data->center = *positionPtr++;
				data->color  = *colorPtr++;
				data->sinCos = *sinCosPtr++;
				data->size   = ComputeSize(*sizePtr++);
				data++;
			}

			billboards.Insert({
				renderOrder,
				material,
				scissorRect,
				billboardCount,
				billboardIndex
			});
		}
	}

	/*!
	* \brief Adds multiple billboards to the queue
	*
	* \param renderOrder Order of rendering
	* \param material Material of the billboards
	* \param count Number of billboards
	* \param positionPtr Position of the billboards
	* \param sizePtr Size of the billboards
	* \param sinCosPtr Rotation of the billboards if null, Vector2f(0.f, 1.f) is used
	* \param alphaPtr Alpha parameters of the billboards if null, 1.f is used
	*
	* \remark Produces a NazaraAssert if material is invalid
	*/

	void ForwardRenderQueue::AddBillboards(int renderOrder, const Material* material, std::size_t billboardCount, const Recti& scissorRect, SparsePtr<const Vector3f> positionPtr, SparsePtr<const float> sizePtr, SparsePtr<const Vector2f> sinCosPtr, SparsePtr<const float> alphaPtr)
	{
		NazaraAssert(material, "Invalid material");

		Vector2f defaultSinCos(0.f, 1.f); // sin(0) = 0, cos(0) = 1

		if (!sinCosPtr)
			sinCosPtr.Reset(&defaultSinCos, 0); // The trick here is to put the stride to zero, which leads the pointer to be immobile

		float defaultAlpha = 1.f;

		if (!alphaPtr)
			alphaPtr.Reset(&defaultAlpha, 0); // Same
		
		if (material->IsDepthSortingEnabled())
		{
			for (std::size_t i = 0; i < billboardCount; ++i)
			{
				depthSortedBillboards.Insert({
					renderOrder,
					material,
					scissorRect,
					{
						ComputeColor(*alphaPtr++),
						*positionPtr++,
						ComputeSize(*sizePtr++),
						*sinCosPtr++
					}
				});
			}
		}
		else
		{
			std::size_t billboardIndex = m_billboards.size();
			m_billboards.resize(billboardIndex + billboardCount);
			BillboardData* data = &m_billboards[billboardIndex];

			for (std::size_t i = 0; i < billboardCount; ++i)
			{
				data->center = *positionPtr++;
				data->color  = ComputeColor(*alphaPtr++);
				data->sinCos = *sinCosPtr++;
				data->size   = ComputeSize(*sizePtr++);
				data++;
			}

			billboards.Insert({
				renderOrder,
				material,
				scissorRect,
				billboardCount,
				billboardIndex
			});
		}
	}

	/*!
	* \brief Adds multiple billboards to the queue
	*
	* \param renderOrder Order of rendering
	* \param material Material of the billboards
	* \param count Number of billboards
	* \param positionPtr Position of the billboards
	* \param sizePtr Size of the billboards
	* \param anglePtr Rotation of the billboards if null, 0.f is used
	* \param colorPtr Color of the billboards if null, Color::White is used
	*
	* \remark Produces a NazaraAssert if material is invalid
	*/

	void ForwardRenderQueue::AddBillboards(int renderOrder, const Material* material, std::size_t billboardCount, const Recti& scissorRect, SparsePtr<const Vector3f> positionPtr, SparsePtr<const float> sizePtr, SparsePtr<const float> anglePtr, SparsePtr<const Color> colorPtr)
	{
		NazaraAssert(material, "Invalid material");

		float defaultRotation = 0.f;

		if (!anglePtr)
			anglePtr.Reset(&defaultRotation, 0); // The trick here is to put the stride to zero, which leads the pointer to be immobile

		if (!colorPtr)
			colorPtr.Reset(&Color::White, 0); // Same
		
		if (material->IsDepthSortingEnabled())
		{
			for (std::size_t i = 0; i < billboardCount; ++i)
			{
				depthSortedBillboards.Insert({
					renderOrder,
					material,
					scissorRect,
					{
						*colorPtr++,
						*positionPtr++,
						ComputeSize(*sizePtr++),
						ComputeSinCos(*anglePtr++)
					}
				});
			}
		}
		else
		{
			std::size_t billboardIndex = m_billboards.size();
			m_billboards.resize(billboardIndex + billboardCount);
			BillboardData* data = &m_billboards[billboardIndex];

			for (std::size_t i = 0; i < billboardCount; ++i)
			{
				data->center = *positionPtr++;
				data->color  = *colorPtr++;
				data->sinCos = ComputeSinCos(*anglePtr++);
				data->size   = ComputeSize(*sizePtr++);
				data++;
			}

			billboards.Insert({
				renderOrder,
				material,
				scissorRect,
				billboardCount,
				billboardIndex
			});
		}
	}

	/*!
	* \brief Adds multiple billboards to the queue
	*
	* \param renderOrder Order of rendering
	* \param material Material of the billboards
	* \param count Number of billboards
	* \param positionPtr Position of the billboards
	* \param sizePtr Size of the billboards
	* \param anglePtr Rotation of the billboards if null, 0.f is used
	* \param alphaPtr Alpha parameters of the billboards if null, 1.f is used
	*
	* \remark Produces a NazaraAssert if material is invalid
	*/

	void ForwardRenderQueue::AddBillboards(int renderOrder, const Material* material, std::size_t billboardCount, const Recti& scissorRect, SparsePtr<const Vector3f> positionPtr, SparsePtr<const float> sizePtr, SparsePtr<const float> anglePtr, SparsePtr<const float> alphaPtr)
	{
		NazaraAssert(material, "Invalid material");

		float defaultRotation = 0.f;

		if (!anglePtr)
			anglePtr.Reset(&defaultRotation, 0); // The trick here is to put the stride to zero, which leads the pointer to be immobile

		float defaultAlpha = 1.f;

		if (!alphaPtr)
			alphaPtr.Reset(&defaultAlpha, 0); // Same
		
		if (material->IsDepthSortingEnabled())
		{
			for (std::size_t i = 0; i < billboardCount; ++i)
			{
				depthSortedBillboards.Insert({
					renderOrder,
					material,
					scissorRect,
					{
						ComputeColor(*alphaPtr++),
						*positionPtr++,
						ComputeSize(*sizePtr++),
						ComputeSinCos(*anglePtr++)
					}
				});
			}
		}
		else
		{
			std::size_t billboardIndex = m_billboards.size();
			m_billboards.resize(billboardIndex + billboardCount);
			BillboardData* data = &m_billboards[billboardIndex];

			for (std::size_t i = 0; i < billboardCount; ++i)
			{
				data->center = *positionPtr++;
				data->color  = ComputeColor(*alphaPtr++);
				data->sinCos = ComputeSinCos(*anglePtr++);
				data->size   = ComputeSize(*sizePtr++);
				data++;
			}

			billboards.Insert({
				renderOrder,
				material,
				scissorRect,
				billboardCount,
				billboardIndex
			});
		}
	}

	/*!
	* \brief Adds drawable to the queue
	*
	* \param renderOrder Order of rendering
	* \param drawable Drawable user defined
	*
	* \remark Produces a NazaraError if drawable is invalid
	*/
	void ForwardRenderQueue::AddDrawable(int renderOrder, const Drawable* drawable)
	{
		NazaraAssert(drawable, "Invalid material");

		RegisterLayer(renderOrder);

		customDrawables.Insert({
			renderOrder,
			drawable
		});
	}

	/*!
	* \brief Adds mesh to the queue
	*
	* \param renderOrder Order of rendering
	* \param material Material of the mesh
	* \param meshData Data of the mesh
	* \param meshAABB Box of the mesh
	* \param transformMatrix Matrix of the mesh
	*
	* \remark Produces a NazaraAssert if material is invalid
	*/
	void ForwardRenderQueue::AddMesh(int renderOrder, const Material* material, const MeshData& meshData, const Boxf& meshAABB, const Matrix4f& transformMatrix, const Recti& scissorRect)
	{
		NazaraAssert(material, "Invalid material");

		RegisterLayer(renderOrder);

		Spheref obbSphere(transformMatrix.GetTranslation() + meshAABB.GetCenter(), meshAABB.GetSquaredRadius());

		if (material->IsDepthSortingEnabled())
		{
			depthSortedModels.Insert({
				renderOrder,
				meshData,
				material,
				transformMatrix,
				scissorRect,
				obbSphere
			});
		}
		else
		{
			models.Insert({
				renderOrder,
				meshData,
				material,
				transformMatrix,
				scissorRect,
				obbSphere
			});
		}
	}

	/*!
	* \brief Adds sprites to the queue
	*
	* \param renderOrder Order of rendering
	* \param material Material of the sprites
	* \param vertices Buffer of data for the sprites
	* \param spriteCount Number of sprites
	* \param overlay Texture of the sprites
	*
	* \remark Produces a NazaraAssert if material is invalid
	*/
	void ForwardRenderQueue::AddSprites(int renderOrder, const Material* material, const VertexStruct_XYZ_Color_UV* vertices, std::size_t spriteCount, const Recti& scissorRect, const Texture* overlay /*= nullptr*/)
	{
		NazaraAssert(material, "Invalid material");

		RegisterLayer(renderOrder);

		if (material->IsDepthSortingEnabled())
		{
			depthSortedSprites.Insert({
				renderOrder,
				spriteCount,
				material,
				overlay,
				vertices,
				scissorRect
				});
		}
		else
		{
			basicSprites.Insert({
				renderOrder,
				spriteCount,
				material,
				overlay,
				vertices,
				scissorRect
			});
		}
	}

	/*!
	* \brief Clears the queue
	*
	* \param fully Should everything be cleared or we can keep layers
	*/

	void ForwardRenderQueue::Clear(bool fully)
	{
		AbstractRenderQueue::Clear(fully);

		basicSprites.Clear();
		billboards.Clear();
		depthSortedBillboards.Clear();
		depthSortedModels.Clear();
		depthSortedSprites.Clear();
		models.Clear();

		m_billboards.clear();
		m_renderLayers.clear();
	}

	/*!
	* \brief Sorts the object according to the viewer position, furthest to nearest
	*
	* \param viewer Viewer of the scene
	*/

	void ForwardRenderQueue::Sort(const AbstractViewer* viewer)
	{
		static std::unordered_map<int, std::size_t> layers;
		static bool once = false;

		if (!once)
		{
			for (int layer : m_renderLayers)
				layers.emplace(layer, layers.size());

			once = true;
		}


		static std::unordered_map<const MaterialPipeline*, std::size_t> pipelines;
		static std::unordered_map<const Material*, std::size_t> materials;
		static std::unordered_map<const Texture*, std::size_t> overlays;
		static std::unordered_map<const UberShader*, std::size_t> shaders;
		static std::unordered_map<const Texture*, std::size_t> textures;
		static std::unordered_map<const VertexBuffer*, std::size_t> vertexBuffers;

		basicSprites.Sort([&](const SpriteChain& vertices)
		{
			// RQ index:
			// - Layer (4bits)
			// - Pipeline (8bits)
			// - Material (8bits)
			// - Shader? (8bits)
			// - Textures (8bits)
			// - Overlay (8bits)
			// - Scissor (4bits)
			// - Depth? (16bits)

			auto GetOrInsert = [](auto& container, auto&& value)
			{
				return container.emplace(value, container.size()).first->second;
			};

			UInt64 layerIndex = layers[vertices.layerIndex];
			UInt64 pipelineIndex = GetOrInsert(pipelines, vertices.material->GetPipeline());
			UInt64 materialIndex = GetOrInsert(materials, vertices.material);
			UInt64 shaderIndex = GetOrInsert(shaders, vertices.material->GetShader());
			UInt64 textureIndex = GetOrInsert(textures, vertices.material->GetDiffuseMap());
			UInt64 overlayIndex = GetOrInsert(overlays, vertices.overlay);
			UInt64 scissorIndex = 0; //< TODO
			UInt64 depthIndex = 0; //< TODO

			UInt64 index = (layerIndex    & 0x0F)   << 60 |
			               (pipelineIndex & 0xFF)   << 52 |
			               (materialIndex & 0xFF)   << 44 |
			               (shaderIndex   & 0xFF)   << 36 |
			               (textureIndex  & 0xFF)   << 28 |
			               (overlayIndex  & 0xFF)   << 20 |
			               (scissorIndex  & 0x0F)   << 16 |
			               (depthIndex    & 0xFFFF) <<  0;

			return index;
		});
		
		billboards.Sort([&](const BillboardChain& billboard)
		{
			// RQ index:
			// - Layer (4bits)
			// - Pipeline (8bits)
			// - Material (8bits)
			// - Shader? (8bits)
			// - Textures (8bits)
			// - ??? (8bits)
			// - Scissor (4bits)
			// - Depth? (16bits)

			auto GetOrInsert = [](auto& container, auto&& value)
			{
				auto it = container.find(value);
				if (it == container.end())
					it = container.emplace(value, container.size()).first;

				return it->second;
			};

			UInt64 layerIndex = layers[billboard.layerIndex];
			UInt64 pipelineIndex = GetOrInsert(pipelines, billboard.material->GetPipeline());
			UInt64 materialIndex = GetOrInsert(materials, billboard.material);
			UInt64 shaderIndex = GetOrInsert(shaders, billboard.material->GetShader());
			UInt64 textureIndex = GetOrInsert(textures, billboard.material->GetDiffuseMap());
			UInt64 unknownIndex = 0; //< ???
			UInt64 scissorIndex = 0; //< TODO
			UInt64 depthIndex = 0; //< TODO?

			UInt64 index = (layerIndex    & 0x0F)   << 60 |
			               (pipelineIndex & 0xFF)   << 52 |
			               (materialIndex & 0xFF)   << 44 |
			               (shaderIndex   & 0xFF)   << 36 |
			               (textureIndex  & 0xFF)   << 28 |
			               (unknownIndex  & 0xFF)   << 20 |
			               (scissorIndex  & 0x0F)   << 16 |
			               (depthIndex    & 0xFFFF) <<  0;

			return index;
		});

		customDrawables.Sort([&](const CustomDrawable& drawable)
		{
			// RQ index:
			// - Layer (4bits)

			UInt64 layerIndex = layers[drawable.layerIndex];

			UInt64 index = (layerIndex & 0x0F) << 60;

			return index;

		});

		models.Sort([&](const Model& renderData)
		{
			// RQ index:
			// - Layer (4bits)
			// - Pipeline (8bits)
			// - Material (8bits)
			// - Shader? (8bits)
			// - Textures (8bits)
			// - Buffers (8bits)
			// - Scissor (4bits)
			// - Depth? (16bits)

			auto GetOrInsert = [](auto& container, auto&& value)
			{
				return container.emplace(value, container.size()).first->second;
			};

			UInt64 layerIndex = layers[renderData.layerIndex];
			UInt64 pipelineIndex = GetOrInsert(pipelines, renderData.material->GetPipeline());
			UInt64 materialIndex = GetOrInsert(materials, renderData.material);
			UInt64 shaderIndex = GetOrInsert(shaders, renderData.material->GetShader());
			UInt64 textureIndex = GetOrInsert(textures, renderData.material->GetDiffuseMap());
			UInt64 bufferIndex = GetOrInsert(vertexBuffers, renderData.meshData.vertexBuffer);
			UInt64 scissorIndex = 0; //< TODO
			UInt64 depthIndex = 0; //< TODO

			UInt64 index = (layerIndex     & 0x0F)   << 60 |
			                (pipelineIndex & 0xFF)   << 52 |
			                (materialIndex & 0xFF)   << 44 |
			                (shaderIndex   & 0xFF)   << 36 |
			                (textureIndex  & 0xFF)   << 28 |
			                (bufferIndex   & 0xFF)   << 20 |
			                (scissorIndex  & 0x0F)   << 16 |
			                (depthIndex    & 0xFFFF) <<  0;

			return index;
		});

		static_assert(std::numeric_limits<float>::is_iec559, "The following sorting functions relies on IEEE 754 floatings-points");

#if defined(arm) && \
    ((defined(__MAVERICK__) && defined(NAZARA_BIG_ENDIAN)) || \
    (!defined(__SOFTFP__) && !defined(__VFP_FP__) && !defined(__MAVERICK__)))
	#error The following code relies on native-endian IEEE-754 representation, which your platform does not guarantee
#endif

		Planef nearPlane = viewer->GetFrustum().GetPlane(FrustumPlane_Near);

		depthSortedBillboards.Sort([&](const Billboard& billboard)
		{
			// RQ index:
			// - Layer (4bits)
			// - Depth (32bits)
			// - ??    (28bits)

			// Reinterpret depth as UInt32 (this will work as long as they're all either positive or negative,
			// a negative distance may happen with billboard behind the camera which we don't care about since they'll be rendered)
			float depth = nearPlane.Distance(billboard.data.center);

			UInt64 layerIndex = layers[billboard.layerIndex];
			UInt64 depthIndex = ~reinterpret_cast<UInt32&>(depth);

			UInt64 index = (layerIndex & 0x0F) << 60 |
			               (depthIndex & 0xFFFFFFFF) << 28;

			return index;
		});

		if (viewer->GetProjectionType() == ProjectionType_Orthogonal)
		{
			depthSortedModels.Sort([&](const Model& model)
			{
				// RQ index:
				// - Layer (4bits)
				// - Depth (32bits)
				// - ??    (28bits)

				float depth = nearPlane.Distance(model.obbSphere.GetPosition());

				UInt64 layerIndex = layers[model.layerIndex];
				UInt64 depthIndex = ~reinterpret_cast<UInt32&>(depth);

				UInt64 index = (layerIndex & 0x0F) << 60 |
				               (depthIndex & 0xFFFFFFFF) << 28;

				return index;
			});

			depthSortedSprites.Sort([&](const SpriteChain& spriteChain)
			{
				// RQ index:
				// - Layer (4bits)
				// - Depth (32bits)
				// - ??    (28bits)

				float depth = nearPlane.Distance(spriteChain.vertices[0].position);

				UInt64 layerIndex = layers[spriteChain.layerIndex];
				UInt64 depthIndex = ~reinterpret_cast<UInt32&>(depth);

				UInt64 index = (layerIndex & 0x0F) << 60 |
				               (depthIndex & 0xFFFFFFFF) << 28;

				return index;
			});
		}
		else
		{
			Vector3f viewerPos = viewer->GetEyePosition();
			
			depthSortedModels.Sort([&](const Model& model)
			{
				// RQ index:
				// - Layer (4bits)
				// - Depth (32bits)
				// - ??    (28bits)

				float depth = viewerPos.SquaredDistance(model.obbSphere.GetPosition());

				UInt64 layerIndex = layers[model.layerIndex];
				UInt64 depthIndex = ~reinterpret_cast<UInt32&>(depth);

				UInt64 index = (layerIndex & 0x0F) << 60 |
				               (depthIndex & 0xFFFFFFFF) << 28;

				return index;
			});

			depthSortedSprites.Sort([&](const SpriteChain& sprites)
			{
				// RQ index:
				// - Layer (4bits)
				// - Depth (32bits)
				// - ??    (28bits)

				float depth = viewerPos.SquaredDistance(sprites.vertices[0].position);

				UInt64 layerIndex = layers[sprites.layerIndex];
				UInt64 depthIndex = ~reinterpret_cast<UInt32&>(depth);

				UInt64 index = (layerIndex & 0x0F) << 60 |
				               (depthIndex & 0xFFFFFFFF) << 28;

				return index;
			});
		}
	}


	bool ForwardRenderQueue::MaterialComparator::operator()(const Material* mat1, const Material* mat2) const
	{
		const Texture* diffuseMap1 = mat1->GetDiffuseMap();
		const Texture* diffuseMap2 = mat2->GetDiffuseMap();
		if (diffuseMap1 != diffuseMap2)
			return diffuseMap1 < diffuseMap2;

		return mat1 < mat2;
	}

	bool ForwardRenderQueue::MaterialPipelineComparator::operator()(const MaterialPipeline* pipeline1, const MaterialPipeline* pipeline2) const
	{
		const Shader* shader1 = pipeline1->GetInstance().renderPipeline.GetInfo().shader;
		const Shader* shader2 = pipeline2->GetInstance().renderPipeline.GetInfo().shader;
		if (shader1 != shader2)
			return shader1 < shader2;

		return pipeline1 < pipeline2;
	}

	/*!
	* \brief Functor to compare two mesh data
	* \return true If first mesh is "smaller" than the second one
	*
	* \param data1 First mesh to compare
	* \param data2 Second mesh to compare
	*/

	bool ForwardRenderQueue::MeshDataComparator::operator()(const MeshData& data1, const MeshData& data2) const
	{
		const Buffer* buffer1;
		const Buffer* buffer2;

		buffer1 = (data1.indexBuffer) ? data1.indexBuffer->GetBuffer() : nullptr;
		buffer2 = (data2.indexBuffer) ? data2.indexBuffer->GetBuffer() : nullptr;
		if (buffer1 != buffer2)
			return buffer1 < buffer2;

		buffer1 = data1.vertexBuffer->GetBuffer();
		buffer2 = data2.vertexBuffer->GetBuffer();
		if (buffer1 != buffer2)
			return buffer1 < buffer2;

		return data1.primitiveMode < data2.primitiveMode;
	}
}
