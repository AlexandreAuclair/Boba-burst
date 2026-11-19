#pragma once

#include "../DataStruct/VulkanDef.h"
#include "../Logger/Logger.h"


class Scene
{
public:
	virtual ~Scene() = default;
	virtual void Update(double dt)
	{
		m_currentTime += dt;
	}
	virtual void Draw(const DrawInfo&) = 0;

protected:
	double m_currentTime = 0;
};
