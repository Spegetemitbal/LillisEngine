#pragma once

namespace LILLIS
{
	template <class Obj>
	class ComponentPool
	{
	public:
		ComponentPool();
		~ComponentPool() { delete mPool; };
		ComponentPool(unsigned int numComp);

		Obj* AddComponent();
		void DestroyComponent(Obj& comp);

		std::vector<Obj*> poolDir;

	private:

		void SortPool();
		void ResizePool();

		//std::vector<Component*> activeCheckDir;
		Obj* AllocateObj(char* base);

		bool isGameObj = false;

		unsigned int activeLine = 0;
		unsigned int numActive = 0;
		unsigned int count = 0;
		char* mPool = nullptr;
	};
}
