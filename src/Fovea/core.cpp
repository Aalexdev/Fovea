#include "Fovea/core.hpp"

namespace Fovea{
	inline Core** getInstancePtr(){
		static Core* core = new Core;
		assert(core != nullptr && "Fovea has already been shutdown");
		return &core;
	}

	Core &getInstance(){
		return **getInstancePtr();
	}

	void initInstance(){
		getInstancePtr();
	}

	void shutdownInstance(){
		auto instance = getInstancePtr();
		delete *instance;
		*instance = nullptr;
	}

	WaitIdle::~WaitIdle(){
		vkDeviceWaitIdle(getInstance().logicalDevice.getDevice());
	}
}