#include "Camera.h"

Camera::Camera() {
	_up = Vec3(0.0f, 1.0f, 0.0f);
	_orientation = Vec3(0.0f, 0.0f, -1.0f);
}

Camera::Camera(unsigned int height, unsigned int width, Vec3 position) : _width(width), _height(height), _position(position){
	_up = Vec3(0.0f, 1.0f, 0.0f);
	_orientation = Vec3(0.0f, 0.0f, -1.0f);

}
Matrix4x4* Camera::GetCamera() {
	return &_viewMatrix;
}