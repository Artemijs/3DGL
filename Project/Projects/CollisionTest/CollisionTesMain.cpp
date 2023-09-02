#include "CollisionTesMain.h"
#include "../../Graphics/Renderer.h"
#include "../../Graphics/Sprite.h"
#include "../../Math/BoxBounds.h"
#include "../../Math/SphereBounds.h"
//
#include "../../UI/Button.h"

CollisionTestMain::CollisionTestMain():Game() {
	
	_myNode = new BaseNode(Vec3(450, 400, 0), Vec3(50, 50, 1), 0);
	Sprite* s = new Sprite("Assets/Textures/Circle.png");
	//s->GetMaterial()
	_myNode->AddComponent(s);
	Bounds* bBox = new SphereBounds(_myNode, 12.5f);
	bBox->_solid = false;		
	_myNode->AddComponent(bBox);
	_world->AddChild(_myNode);	

	
	unsigned int maxNodes = 10;
	_otherNodes = new BaseNode[maxNodes];
	int windowW = Renderer::instance()->GetWindowSize().x;
	int windowH = Renderer::instance()->GetWindowSize().y;

	for (int i = 0; i < maxNodes; i++) {

		Vec3 randPos = Vec3(rand() % windowW, rand() % windowH, 0);
		int rRad = 50;// rand() % 100;
		BaseNode* b = new BaseNode(randPos, Vec3(rRad, rRad, 1), 0);
		b->AddComponent(new Sprite("Assets/Textures/Circle.png"));
		Bounds* bBox = new SphereBounds(b, 12.5f);
		bBox->_solid = false;
		b->AddComponent(bBox);
		
		_otherNodes->AddChild(b);
		_world->AddChild(b);
	}



/*

	Bounds* aBox = _myNode->GetComponent<Bounds>();
	aBox->AddEvent(true, new CollisionEvent(bBox, []( Bounds* a,  Bounds* b, SeparationData& sd) {
		printf("HELLO WORLD from a collisiion event\n");
	}));


	aBox->AddEvent(false, new CollisionEvent(bBox, [](Bounds* a, Bounds* b, SeparationData& sd) {
		printf("EXIT COLLISION EVENTO\n");
	}));
	aBox->_isFixed = true;
	//b->AddComponent(new Sprite("Assets/Textures/default.png"));
	//b->AddComponent(new BoxBounds());
	
	
	_world->AddChild(a);
	
	_world->AddChild(b);
	*/
	//CreateTests(a, b);
	//_state = 0;
	_play = false;

	_angVel = 0;
	_ang = 0;
	//_angChange = 2;



/*BaseNode* circle = new BaseNode(Vec3(100, 100, 0), Vec3(50, 50, 1), 0);
	circle->AddComponent(new Sprite("Assets/Textures/Circle.png"));
	Bounds* crclBounds = new SphereBounds(circle, 25);
	
	circle->AddComponent(crclBounds);
	_world->AddChild(circle);*/


}


CollisionTestMain::~CollisionTestMain() {
	std::cout << "deleting CollisionDetection\n";
	delete _world;
	delete _otherNodes;
}
//263 left
//262 right 
// 32 space 
#include "../../Math/CollisionDetection.h"
void CollisionTestMain::HandleKeyInputs(int key, int action, int mods) {
	if (action == 2) return;
	if (action == 0) {
		std::cout << " key event called from tower defense " << "aaction " << action << " key " << key << " mods " << mods << "\n";
		switch (key)
		{
		case 49:
			CollisionDetection::_print = true;
			//left
		case 263:
			//Rotate(1);
			break;
			//right
		case 262:
			//Rotate(-1);
			break;
			//space
		case 32:
			Play(!_play);
			break;
		case 80://n
			//PrevTest();
			break;
		case 78://p
			//NextTest();
			break;
		default:
			break;
		}
		
	}
}

void CollisionTestMain::Update(float deltaTime) {
	Game::Update(deltaTime);
	if (!_play) return;
	



}

void CollisionTestMain::Play(const bool on) {
	_play = on;
}

