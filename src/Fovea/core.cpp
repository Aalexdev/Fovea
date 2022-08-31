#include "Fovea/core.hpp"

namespace Fovea{
	inline Core** getInstancePtr(){
		static Core* core = new Core;

		#ifndef NDEBUG
			if (core == nullptr){
				throw "Fovea has already been shutdown";
			}
		#endif

		return &core;
	}

	Core &getInstance(){
		return **getInstancePtr();
	}

	void initializeInstance(){
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