#include "HeroEntity.h"
#include "opencv2/opencv.hpp"
#include "Config.h"

#include <memory>
#include <filesystem>

using namespace cv;
using namespace std;
namespace fs = std::filesystem;

enum HeroStates {
	HERO_IDLE,
	HERO_RUN_RIGHT,
	HERO_RUN_LEFT,
	HERO_DUCK, 
	HERO_STAY_DUCK, 
	HERO_STAND_AFTER_DUCK, 
	HERO_JUMP 
};

EntityStatePtr createHeroState(
	fs::path const& animationFolder,
	HeroStates state)
{
	AnimationPtr animation(new Animation(animationFolder.string()));
	bool isNotCyclic =
		state == HeroStates::HERO_DUCK ||
		state == HeroStates::HERO_STAND_AFTER_DUCK ||
		state == HeroStates::HERO_JUMP;
	bool isCyclic = !isNotCyclic;

	//IGraphicsComponentPtr graphicsPtr(
	//	new SingleAnimationGraphics(animation, isCyclic));

	////IPhysicsComponentPtr physicsPtr = nullptr;
	//Size frameSize = graphicsPtr->getCollisionMask().size();

	IGraphicsComponentPtr graphicsPtr(new SingleAnimationGraphics(animation, isCyclic));
	IPhysicsComponentPtr physicsPtr = nullptr;

	Size frameSize = graphicsPtr->getCollisionMask().size();
	switch (state)
	{
	case HERO_IDLE:
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	case HERO_RUN_RIGHT:
		physicsPtr.reset(new ConstVelocityPhysics(Point(frameSize.width / 10, 0)));
		break;
	case HERO_RUN_LEFT:
		// @4: TODO
		physicsPtr.reset(new ConstVelocityPhysics(Point(-frameSize.width / 10, 0)));
		break;
	case HERO_DUCK:
		// @4: TODO
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	case HERO_STAY_DUCK:
		// @4: TODO
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	case HERO_STAND_AFTER_DUCK:
		// @4: TODO
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	case HERO_JUMP:
		// @4: TODO
		physicsPtr.reset(new JumpPhysics(frameSize.width/5,frameSize.height/3, frameSize.width /10));
		break;
	default:
		throw std::exception("Unknown physics state!");
	}
	IPhysicsComponentPtr BoundedPhysicsDecorator1(new BoundedPhysicsDecorator(physicsPtr));
	return make_shared<EntityState>(graphicsPtr, BoundedPhysicsDecorator1);
	//return make_shared<EntityState>(graphicsPtr, physicsPtr);
}

EntityPtr createHero(std::string const & rootAnimationsFolder)
{
	fs::path root = rootAnimationsFolder;
	auto idleFolder = root / "idle";

	auto idle = createHeroState(root / "idle", HeroStates::HERO_IDLE);
	auto runRight = createHeroState(root / "runRight", HeroStates::HERO_RUN_RIGHT);
	auto jump = createHeroState(root / "jump", HeroStates::HERO_JUMP);
	auto runLeft=createHeroState(root / "runLeft", HeroStates::HERO_RUN_LEFT);
	auto duck = createHeroState(root / "duckDown", HeroStates::HERO_DUCK);
	auto stayDuck = createHeroState(root / "stayDuck", HeroStates::HERO_STAY_DUCK);
	auto standAfterDuck = createHeroState(root / "standAfterDuck", HeroStates::HERO_STAND_AFTER_DUCK);
	// @4: TOOD: add states: runLeft, duck, stayDuck, standAfterDuck

	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_RIGHT }, runRight);
	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_DOWN }, duck);
	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, jump);
	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_LEFT }, runLeft);

	runRight->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_LEFT }, idle);
	runRight->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_DOWN }, duck );
	runRight->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, jump);

	jump->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::ENTITY_PHYSICS_FINISHED }, idle);

	runLeft->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_LEFT }, idle);
	runLeft->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_DOWN }, duck);
	runLeft->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, jump);

	duck->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_GRAPHICS, EventCodes::ENTITY_FINISHED_ANIMATION }, stayDuck);

	stayDuck->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, standAfterDuck);

	standAfterDuck->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_GRAPHICS,EventCodes::ENTITY_FINISHED_ANIMATION }, idle);
	//  @4: TOOD: add all events here... 
	
	// ...    
	EntityPtr hero(new Entity(idle));

	idle->Register(hero);
	runRight->Register(hero);
	jump->Register(hero);
	runLeft->Register(hero);
	duck->Register(hero);
	stayDuck->Register(hero);
	standAfterDuck->Register(hero);
	//  @4: TOOD: add all states here... 

	return hero;
}