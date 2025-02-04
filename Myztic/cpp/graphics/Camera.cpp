#include <pch.h>

#include <display\Window.h>
#include <Scene.h>
#include <graphics\Camera.h>

//glm::vec3 Camera::UP = glm::vec3(0, 1, 0);
//glm::vec3 Camera::ZERO = glm::vec3(0, 0, 0);
//glm::vec3 Camera::FRONT = glm::vec3(0, 0, -1);

Myztic::Camera::Camera(Myztic::ProjectionType type, Scene* linkedScene, glm::vec3 world_position, glm::vec3 look_at, GLfloat near_plane, GLfloat far_plane, GLfloat fov) : cam_position(world_position), look_at(look_at), projection_type(type), linkedScene(linkedScene), near_plane(near_plane), far_plane(far_plane), fov(fov)
{
	update_intern_matrix();
	update_intern_projection();
}