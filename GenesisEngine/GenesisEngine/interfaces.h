#pragma once

#include <algorithm>

#include "Types.h"
#include "Transformation.h"

class Actor;
class ActorComponent;

typedef unsigned int ActorId;
typedef unsigned int ComponentId;

const ActorId INVALID_ACTOR_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

typedef std::tr1::shared_ptr<Actor> StrongActorPtr;
typedef std::tr1::weak_ptr<Actor> WeakActorPtr;
typedef std::tr1::shared_ptr<ActorComponent> StrongActorComponentPtr;
typedef std::tr1::weak_ptr<ActorComponent> WeakActorComponentPtr;

template<class T>
struct SortBy_SharedPtr_Content
{
	bool operator()(const std::tr1::shared_ptr<T> &lhs, const std::tr1::shared_ptr<T> $rhs) const
	{
		return *lhs < *rhs;
	}
};

//
// class IScreenElement -- An interface for an element that will be rendered.
//
class IScreenElement
{
public:
	virtual HRESULT VOnRestore() = 0;
	virtual HRESULT VOnLostDevice() = 0;
	virtual HRESULT VOnRender(double fTime, float fElapsedTime) = 0;
	virtual void VOnUpdate(int deltaMilliseconds) = 0;

	virtual int VGetZOrder() const = 0;
	virtual void VSetZOrder(int const zOrder) = 0;
	virtual bool VIsVisible() const = 0;
	virtual void VSetVisible(bool visible) = 0;

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) = 0;

	virtual ~IScreenElement() { };
	virtual bool const operator <(IScreenElement const &other) { return VGetZOrder() < other.VGetZOrder(); }
};

// Interface for the game physics
class IGamePhysics;

//
// class IGameLogic -- Interface for the base game logic
//
class IGameLogic
{
public:
	virtual WeakActorPtr VGetActor(const ActorId id) = 0;
	virtual StrongActorPtr VCreateActor(const std::string &actorResource, /*const Mat4x4 *initialTransform = NULL,*/ const ActorId serversActorId = INVALID_ACTOR_ID) = 0;
	virtual void VDestroyActor(const ActorId actorId) = 0;
	virtual bool VLoadGame(const char* levelResource) = 0;
	virtual void VSetProxy() = 0;
	virtual void VOnUpdate(float time, float elapsedTime) = 0;
	virtual void VChangeState(enum BaseGameState newState) = 0;
	virtual void VMoveActor(const ActorId id/*, Mat4x4 const &mat*/) = 0;
	virtual std::tr1::shared_ptr<IGamePhysics> VGetGamePhysics(void) = 0;
};

enum GameViewType
{
	GameView_Human,
	GameView_Remote,
	GameView_AI,
	GameView_Recorder,
	GameView_Other
};

typedef unsigned int GameViewId;
extern const GameViewId gc_InvalidGameViewId;

class IGameView
{
public:
	virtual HRESULT VOnRestore() = 0;
	virtual void VOnRender(double fTime, float fElapsedTime) = 0;
	virtual HRESULT VOnLostDevice() = 0;
	virtual GameViewType VGetType() = 0;
	virtual GameViewId VGetId() const = 0;
	virtual void VOnAttach(GameViewId vid, ActorId aid) = 0;

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) = 0;
	virtual void VOnUpdate(unsigned long deltaMs) = 0;

	virtual ~IGameView() { };
};

typedef std::list<shared_ptr<IScreenElement> > ScreenElementList;
typedef std::list<shared_ptr<IGameView> > GameViewList;

// These are the public APIs for any object that implements reactions
// to events sent by hardware user interface devices.
class IKeyboardHandler
{
public:
	virtual bool VOnKeyDown(const BYTE c) = 0;
	virtual bool VOnKeyUp(const BYTE c) = 0;
};

class IPointerHandler
{
public:
	virtual bool VOnPointerMove(const Point& pos, const int radius) = 0;
	virtual bool VOnPointerButtonDown(const Point &pos, const int radius, const std::string &buttonName) = 0;
	virtual bool VOnPointerButtonUp(const Point &pos, const int radius, const std::string &buttonName) = 0;
};

class IJoystickHandler
{
	virtual bool VOnButtonDown(const std::string &buttonName, int const pressure) = 0;
	virtual bool VOnButtonUp(const std::string &buttonName) = 0;
	virtual bool VOnJoystick(float const x, float const y) = 0;
};

class IGamepadHandler
{
	virtual bool VOnTrigger(const std::string &triggerName, float const pressure) = 0;
	virtual bool VOnButtonDown(const std::string &buttonName, int const pressure) = 0;
	virtual bool VOnButtonUp(const std::string &buttonName) = 0;
	virtual bool VOnDirectionalPad(const std::string &direction) = 0;
	virtual bool VOnThumbstick(const std::string &stickName, float const x, float const y) = 0;
};

class Resource;
class IResourceFile;
class ResHandle;

//
// class IResourceLoader
//
class IResourceLoader
{
public:
	virtual std::string VGetPattern() = 0;
	virtual bool VUseRawFile() = 0;
	virtual bool VDiscardRawBufferAfterLoad() = 0;
	virtual bool VAddNullZero() { return false; }
	virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) = 0;
	virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle) = 0;
};

class IResourceFile
{
public:
	virtual bool VOpen() = 0;
	virtual int VGetRawResourceSize(const Resource &r) = 0;
	virtual int VGetRawResource(const Resource &r, char *buffer) = 0;
	virtual int VGetNumResource() const = 0;
	virtual std::string VGetResourceName(int num) const = 0;
	virtual bool VIsUsingDevelopmentDirectories(void) const = 0;
	virtual ~IResourceFile() {}
};

// IScriptManager -- The public interface for the scripting system.
class IScriptManager
{
public:
	virtual ~IScriptManager(void) {}
	virtual bool VInit(void) = 0;
	virtual void VExecuteFile(const char *resource) = 0;
	virtual void VExecuteString(const char *str) = 0;
};

// Rendering

enum RenderPass
{
	RenderPass_0,
	RenderPass_Static = RenderPass_0,
	RenderPass_Actor,
	RenderPass_Sky,
	RenderPass_NotRendered,
	RenderPass_Last
};




class Scene;
class SceneNodeProperties;
class RayCast;
class LightNode;

typedef std::list<shared_ptr<LightNode> > Lights;


class IRenderState
{
public:
	virtual std::string VToString() = 0;
};

//
// class IRenderer				- not described in the 4th edition
//
// class IRenderer abstracts the renderer implementation so the engine
//   can use D3D9 or D3D11.
//
class IRenderer
{
public:
	virtual void VSetBackgroundColor(BYTE bgA, BYTE bgR, BYTE bgG, BYTE bgB) = 0;
	virtual HRESULT VOnRestore() = 0;
	virtual void VShutdown() = 0;
	virtual bool VPreRender() = 0;
	virtual bool VPostRender() = 0;
	virtual void VCalcLighting(Lights *lights, int maximumLights) = 0;
	virtual void VSetWorldTransform(const Transformation2d t) = 0;
	virtual void VSetViewTransform(const Transformation2d t) = 0;
	virtual void VSetProjectionTransform(const Transformation2d t) = 0;
	virtual shared_ptr<IRenderState> VPrepareAlphaPass() = 0;
	virtual shared_ptr<IRenderState> VPrepareSkyBoxPass() = 0;
	virtual void VDrawLine(const Vector2& from, const Vector2& to /*, const Color& color*/) = 0;
};

/////////////////////////////////////////////////////////////////////////////
// class ISceneNode					- Chapter 16, page 524
//
//   This is the public interface for nodes in a 3D scene graph.
/////////////////////////////////////////////////////////////////////////////

class ISceneNode
{
public:
	virtual const SceneNodeProperties * const VGet() const = 0;

	virtual void VSetTransform(const Transformation2d *toWorld, const Transformation2d *fromWorld = NULL) = 0;

	virtual HRESULT VOnUpdate(Scene *pScene, DWORD const elapsedMs) = 0;
	virtual HRESULT VOnRestore(Scene *pScene) = 0;

	virtual HRESULT VPreRender(Scene *pScene) = 0;
	virtual bool VIsVisible(Scene *pScene) const = 0;
	virtual HRESULT VRender(Scene *pScene) = 0;
	virtual HRESULT VRenderChildren(Scene *pScene) = 0;
	virtual HRESULT VPostRender(Scene *pScene) = 0;

	virtual bool VAddChild(shared_ptr<ISceneNode> kid) = 0;
	virtual bool VRemoveChild(ActorId id) = 0;
	virtual HRESULT VOnLostDevice(Scene *pScene) = 0;
	virtual HRESULT VPick(Scene *pScene, RayCast *pRayCast) = 0;


	virtual ~ISceneNode() { };
};