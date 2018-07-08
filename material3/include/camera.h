#include<cglm/cglm.h>
#include<glad/glad.h>

#ifndef MY_COMMON_CAMERA_H
#define MY_COMMON_CAMERA_H
struct Camera {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 center;
    mat4 view;
    float move_speed;
    float rotate_sensitivity;
    float rotate_pitch;
    float rotate_yaw;
    float fov;
};

enum ROTATE_DIRECTION {
    ROTATE_UP,
    ROTATE_DOWN,
    ROTATE_LEFT,
    ROTATE_RIGHT
};

enum MOVE_DIRECTION {
    MOVE_FORWARD,
    MOVE_BACK,
    MOVE_LEFT,
    MOVE_RIGHT
};

struct Camera* init_camera();
GLfloat* camera_lookat(struct Camera *camera);
void camera_move(struct Camera *camera, enum MOVE_DIRECTION d, float delta);
void camera_rotate(struct Camera *camera, float xoffset, float yoffset);
void camera_zoom(struct Camera *camera, float xoffset, float yoffset);
#endif