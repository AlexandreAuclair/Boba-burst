#pragma once

#include "../DataStruct/VulkanDef.h"

class Controller{};
class Camera{};

class Scene
{
public:
	virtual ~Scene() = default;
	virtual void Input(const Controller&, Camera& cam) = 0;
	virtual void Update(double dt, const Camera& cam)
	{
		m_currentTime += dt;
	}
	virtual void Draw(const DrawInfo&) = 0;

protected:
	double m_currentTime = 0;
};
