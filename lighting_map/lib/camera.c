#include <mycommon/camera.h>

struct Camera* init_camera() {
    struct Camera *camera = (struct Camera*)malloc(sizeof(struct Camera));
    camera->position[0] = 0.0f;
    camera->position[1] = 0.0f;
    camera->position[2] = 3.0f;
    camera->move_speed = 0.5f;
    camera->front[0] = 0.0f;
    camera->front[1] = 0.0f;
    camera->front[2] = -1.0f;
    camera->up[0] = 0.0f;
    camera->up[1] = 1.0f;
    camera->up[2] = 0.0f;
    camera->rotate_sensitivity = 0.05;
    camera->fov = 45.0f;
    return camera;
}

GLfloat* camera_lookat(struct Camera *camera) {
    glm_mat4_identity(camera->view);
    glm_vec_add(camera->front, camera->position, camera->center);
    glm_lookat(camera->position, camera->center, camera->up, camera->view);
    return camera->view[0];
}

void camera_move(struct Camera *camera, enum MOVE_DIRECTION d, float delta) {
    float c_speed = delta * camera->move_speed;
    if (MOVE_FORWARD == d) {
        vec3 move;
        glm_vec_scale(camera->front, c_speed, move);
        glm_vec_add(camera->position, move, camera->position);
    }
    if (MOVE_BACK == d) {
        vec3 move;
        glm_vec_scale(camera->front, -c_speed, move);
        glm_vec_add(camera->position, move, camera->position);
    }
    if (MOVE_LEFT == d) {
        vec3 right;
        glm_cross(camera->up, camera->front, right);
        glm_normalize(right);
        glm_vec_scale(right, c_speed, right);
        glm_vec_add(camera->position, right, camera->position);
    }
    if (MOVE_RIGHT == d) {
        vec3 right;
        glm_cross(camera->up, camera->front, right);
        glm_normalize(right);
        glm_vec_scale(right, -c_speed, right);
        glm_vec_add(camera->position, right, camera->position);
    }
}

void camera_rotate(struct Camera *camera, float xoffset, float yoffset) {
    xoffset *= camera->rotate_sensitivity;
    yoffset *= camera->rotate_sensitivity;

    camera->rotate_pitch += yoffset;
    camera->rotate_yaw += xoffset;

    if (camera->rotate_pitch > 89.0f) {
        camera->rotate_pitch = 89.0f;
    }
    if (camera->rotate_pitch < -89.0f) {
        camera->rotate_pitch = -89.0f;
    }

    vec3 f = {0.0f, 0.0f, 0.0f};
    f[0] = cos(glm_rad(camera->rotate_yaw)) * cos(glm_rad(camera->rotate_pitch));
    f[1] = sin(glm_rad(camera->rotate_pitch));
    f[2] = sin(glm_rad(camera->rotate_yaw)) * cos(glm_rad(camera->rotate_pitch));
    glm_normalize_to(f, camera->front);
}

void camera_zoom(struct Camera *camera, float xoffset, float yoffset) {
    if (camera->fov >= 1.0f && camera->fov <= 45.0f) {
        camera->fov -= yoffset;
    }
    if (camera->fov <= 1.0f) {
        camera->fov = 1.0f;
    }
    if (camera->fov >= 45.0f) {
        camera->fov = 45.0f;
    }
}