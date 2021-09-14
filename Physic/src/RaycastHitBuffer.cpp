#include "pch.h"

#include "RaycastHitBuffer.h"

namespace NIX::Physic
{
	bool RaycastHitBuffer::HasBlock() const
	{
		return originalBuffer.hasBlock;
	}

	bool RaycastHitBuffer::HasAnyHit()
	{
		return originalBuffer.hasAnyHits();
	}

	int RaycastHitBuffer::GetNbAnyHits() const
	{
		return originalBuffer.getNbAnyHits();
	}

	int RaycastHitBuffer::GetNbTouches() const
	{
		return originalBuffer.getNbTouches();
	}

	const RaycastHit& RaycastHitBuffer::GetBlockInfo() const
	{
		return block;
	}

	RaycastHit* RaycastHitBuffer::GetTouchesInfo() const
	{
		return touches;
	}

	RaycastHit RaycastHitBuffer::GetTouchInfo(unsigned int index) const
	{
		if (index < originalBuffer.getNbTouches())
		{
			return touches[index];
		}
		return RaycastHit();
	}

	PxRaycastBuffer& RaycastHitBuffer::GetOriginalBuffer()
	{
		return originalBuffer;
	}

	void RaycastHitBuffer::UpdateRaycastHitInfo()
	{
		if (HasBlock())
		{
			block.SetRaycastHitInfo(&originalBuffer.block);
		}

		for (unsigned int index = 0; index < originalBuffer.getNbTouches(); index++)
		{
			touches[index].SetRaycastHitInfo(&originalBuffer.touches[index]);
		}
	}
}
