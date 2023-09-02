#include "CollisionHandler.h"
#include "CollisionDetection.h"
std::vector< std::pair<Bounds*, BaseNode*>>*  CollisionHandler::_all_bounds = new std::vector< std::pair<Bounds*, BaseNode*>>();

void CollisionHandler::RegisterBounds( Bounds* b, BaseNode* bn) {
	_all_bounds->push_back(std::pair<Bounds*, BaseNode*>(b,bn));
}
/// <summary>
/// TO DO: COLLISION LAYERS
/// </summary>
/// <param name="deltaTime"></param>
void CollisionHandler::Update(const float deltaTime) {	
	//check every bounds against every other bounds		
	for (int i = 0; i < _all_bounds->size()-1; ++i) {		
		Bounds* a = _all_bounds->at(i).first;			
		for (int j = i + 1; j < _all_bounds->size(); ++j) {	
			  Bounds* b = _all_bounds->at(j).first;		
			  const unsigned int collisionExists = a->IsColliding(b);
			if (a == b)									
				continue;								
			SeparationData sd = CollisionDetection::CheckCollision(a, b);
			if (sd._penetrationDistance !=0 ) {			
				//collision has happened				
				//no collision with this object happened last frame
				if (collisionExists == -1) {			
					if (a->_solid && b->_solid) {
						a->AddActiveCollision(b, sd);
						b->AddActiveCollision(a, sd);
					}
				}
				CollisionSeparation(_all_bounds->at(i), _all_bounds->at(j), sd);

			}
			else  if(sd._penetrationDistance == 0 && collisionExists != -1){
				//check if you have been colliding with b previously but have not yet called OnExitCollision
				if (collisionExists != -1) {
					printf("EXITING COLLISION\n");
					a->RemoveActiveCollision(b);
				}
			}
		}
	}
}

void CollisionHandler::CollisionSeparation(std::pair<Bounds*, BaseNode*>& a, std::pair<Bounds*, BaseNode*>& b, SeparationData& sd) {
	printf("COLLIDEDD\n");
	//return;
	//check if the object cannot be moved
	//if both objects can be moved then part them equally and separate directions
	//if only one object can be moved part that one object fully



	//move b by half of pen dist 
	if (b.first->_solid) {
		Transform bt = b.second->GetTransform();
		b.second->SetPosition(bt._position + sd._separationVector * sd._penetrationDistance);
	}
	//move a by the other half
	if (a.first->_solid) {
		Transform at = a.second->GetTransform();
		a.second->SetPosition(at._position + sd._separationVector * sd._penetrationDistance);
	}

}

void CollisionHandler::CallEvents(Bounds* a, Bounds* b, const SeparationData* sd) {
	if (sd->_penetrationDistance > 0) {//colllision true
		//a->OnCollisionEnter();
	}
	else {//collision false
	
	}
}



void CollisionHandler::Delete() {
	delete _all_bounds;
}


/*
	spacial hashing 
	collsision between different shape types


*/