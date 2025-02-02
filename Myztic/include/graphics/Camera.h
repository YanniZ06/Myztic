#pragma once
#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum ProjectionType {
	Orthographic,
	Perspective
};

class Scene;

class Camera {
	friend class Scene;
	friend class Drawable;
public:
	//Unit UP vector
	static inline glm::vec3 UP = glm::vec3(0, 1, 0);
	//Zero vector
	static inline glm::vec3 ZERO = glm::vec3(0, 0, 0);
	//Unit negative Z vector
	static inline glm::vec3 FRONT = glm::vec3(0, 0, -1);
	/*
	* Instantiate a camera class, although slightly misleading as it contains the projection aswell.
	* \param projection_type Either an Orthographic projection or a Perspective projection (2D and 3D preferred respectively.)
	* \param scene The current active scene.
	* \param world_position Position the camera should initially be set to, could be changed at any time.
	* \param look_at The general direction the camera should be looking at, should be a unit/normalized vector as it gets added to the position vector.
	* \param near_plane Which is the closest distance the camera can see an object.
	* \param far_plane The farthest distance the camera can see an object.
	* \param fov The angle of the field of view (normal values, 45, 90, 120..) (IGNORABLE IF IT'S AN ORTHOGRAPHIC PROJECTION)
	*/
	Camera(ProjectionType projection_type, Scene* scene, glm::vec3 world_position = glm::vec3(), glm::vec3 look_at = glm::vec3(), GLfloat near_plane = 0.001f, GLfloat far_plane = 1000.f, GLfloat fov = 90.f);
	/*
	* Set the position's x value
	* \param x The new x value
	*/
	inline void set_pos_x(GLfloat x) {
		cam_position.x = x;
		update_intern_matrix();
	};
	/*
	* Set the position's y value
	* \param y The new y value
	*/
	inline void set_pos_y(GLfloat y) {
		cam_position.y = y;
		update_intern_matrix();
	};
	/*
	* Set the position's z value
	* \param z The new z value
	*/
	inline void set_pos_z(GLfloat z) {
		cam_position.z = z;
		update_intern_matrix();
	};
	/*
	* Set the position's value
	* \param world_position The new position value
	*/
	inline void set_position(glm::vec3 world_position) {
		cam_position = world_position;
		update_intern_matrix();
	};
	/*
	* Set the look direction's x value
	* \param x The new x value
	*/
	inline void set_look_x(GLfloat x) {
		look_at.x = x;
		update_intern_matrix();
	};
	/*
	* Set the look directions's y value
	* \param y The new y value
	*/
	inline void set_look_y(GLfloat y) {
		look_at.y = y;
		update_intern_matrix();
	};
	/*
	* Set the look directions's z value
	* \param z The new z value
	*/
	inline void set_look_z(GLfloat z) {
		look_at.z = z;
		update_intern_matrix();
	};
	/*
	* Set the look directions' value
	* \param look_at The new direction to look at.
	*/
	inline void set_look(glm::vec3 look_at) {
		this->look_at = look_at;
		update_intern_matrix();
	};
	/*
	* Set the FOV 
	* \param fov The new FOV to be inherited by the projection.
	*/
	inline void set_fov(GLfloat fov) {
		if (projection_type == Orthographic) return;
		fov = fov;
		update_intern_projection();
	};
	/*
	* Set the projection's type.
	* \param proj The type of the new projection.
	*/
	inline void set_projectionType(ProjectionType proj) {
		this->projection_type = proj;
		update_intern_projection();
	};
	/*
	* Set the near plane's minimum distance
	* \param near_p The magnitude of the minimum distance
	*/
	inline void set_nearPlane(GLfloat near_p) {
		near_plane = near_p;
		update_intern_projection();
	};
	/*
	* Set the far plane's maximum distance from the observer.
	* \param far_p The magnitude of the maximum distance
	*/
	inline void set_farPlane(GLfloat far_p) {
		far_plane = far_p;
		update_intern_projection();
	};
	/*
	* A callback to update the projection which is dependent on the window's size.
	*/
	inline void on_screenResize() {
		update_intern_projection();
	};
	/*
	* Returns a copy of the camera position.
	*/
	inline glm::vec3 get_position() {
		return cam_position;
	};
	/*
	* Returns a copy of the camera direction.
	*/
	inline glm::vec3 get_look_at() {
		return look_at;
	};
	/*
	* Returns a copy of the FOV
	*/
	inline GLfloat get_fov() {
		return fov;
	};
	/*
	* Returns a copy of the near plane's distance magnitude 
	*/
	inline GLfloat get_near_plane() {
		return near_plane;
	};
	/*
	* Returns a copy of the far plane's distance magnitude
	*/
	inline GLfloat get_far_plane() {
		return far_plane;
	};
public:
	ProjectionType projection_type;
private:
	glm::vec3 cam_position;
	glm::vec3 look_at;

	GLfloat fov = 90.f;
	GLfloat near_plane = 0.001f;
	GLfloat far_plane = 1000.f;
	Scene* linkedScene;
	inline void update_intern_matrix() {
		//translates the vector to the direction of the camera position so the look at isnt stuck at one place, unless we wanted so (lock in system i guess)
		//could implement that by different means, perhaps? for now, look_at is probably gonna be passed in as FRONT so you may only see in FRONT of you
		intern_ViewMatrix = glm::lookAt(cam_position, cam_position + look_at, UP);
	};
	inline void update_intern_projection() {
		switch (projection_type) {
		case Orthographic:
			//opengl is right handed btw so this is equivalent to orthoRH
			this->intern_ProjecMatrix = glm::ortho(0.0f, static_cast<GLfloat>(linkedScene->loadedWin->w()), static_cast<GLfloat>(linkedScene->loadedWin->h()), 0.0f, near_plane, far_plane);
			break;
		case Perspective:
			this->intern_ProjecMatrix = glm::perspective(glm::radians(fov), static_cast<GLfloat>(linkedScene->loadedWin->w()) / static_cast<GLfloat>(linkedScene->loadedWin->h()), near_plane, far_plane);
			break;
		};
	};
protected:
	glm::mat4 intern_ProjecMatrix;
	glm::mat4 intern_ViewMatrix;
};