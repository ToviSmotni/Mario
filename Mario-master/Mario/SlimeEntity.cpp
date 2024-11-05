#include "SlimeEntity.h"
#include <filesystem>

using namespace cv;
using namespace std;
namespace fs = std::filesystem;

enum SlimeState {
	SLIME_BOMP
};
EntityPtr createSlime(std::string const& animationFolder)
{

	fs::path root = animationFolder;
	auto SlimeFolder = root / "SlimeOrange";

	auto Myslime = CreateSlimeEnemy(animationFolder);
	
	EntityPtr slime(new Entity(Myslime));
	return slime;
}

EntityStatePtr CreateSlimeEnemy(std::string const& animationFolder)
{
	AnimationPtr animation(new Animation(animationFolder));
	bool isCyclic = true;

	IGraphicsComponentPtr graphicsPtr(
		new SingleAnimationGraphics(animation, isCyclic));

	IPhysicsComponentPtr physicsPtr = make_shared<FixedWidgetPhysics>();

	return make_shared<EntityState>(graphicsPtr, physicsPtr);
}