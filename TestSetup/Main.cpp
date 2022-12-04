#include "../Editor/EditorApplication.h"
#include "../GameplaySystem/TextureLoader.h"

int main(int argc, char* argv[]) {
	if (HRESULT result = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED); FAILED(result)) {
		return result;
	}

	JPH::RegisterDefaultAllocator();

	//StandaloneGameTestApplication app(kDllPath);
	EditorApplication app;
	return app.Run(argc,argv);
}
