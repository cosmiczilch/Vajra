#ifndef ENGINE_H
#define ENGINE_H

int testEngineFunction();

// Forward Declarations:
class AssetLibrary;
class MessageHub;
class SceneGraph;
class Timer;
class Tween;
class Input;

class Engine {
public:
	~Engine();

	static Engine* GetInstance();

	inline AssetLibrary* GetAssetLibrary() { return this->assetLibrary; }
	inline MessageHub*   GetMessageHub()   { return this->messageHub;   }
	inline SceneGraph*   GetSceneGraph()   { return this->sceneGraph;   }
	inline Timer*        GetTimer()        { return this->timer;        }
	inline Tween*        GetTween()		   { return this->tween;        }
	inline Input*		 GetInput()		   { return this->input;		}

	void DoFrame();

private:
	// Make this a singleton:
	Engine();
	static Engine *singleton;
	//
	void init();
	void destroy();

	void updateScene();
	void renderScene();

	AssetLibrary* assetLibrary;
	MessageHub* messageHub;
	SceneGraph* sceneGraph;
	Timer* timer;
	Tween* tween;
	Input* input;
};

#define ENGINE Engine::GetInstance()

#endif // ENGINE_H
