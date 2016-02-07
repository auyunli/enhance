#ifndef RENDERTYPE_H
#define RENDERTYPE_H

enum class eRenderType {
    POLY_VERT,
    POLY_NORM,
    LIGHT_COEFF_AMBIENT,
    LIGHT_COEFF_DIFFUSE,
    LIGHT_COEFF_SPECULAR,
    LIGHT_COORDINATE,
    LIGHT_DIRECTION,
    MATERIAL_COEFF_AMBIENT,
    MATERIAL_COEFF_DIFFUSE,
    MATERIAL_COEFF_SPECULAR,
    MATERIAL_COEFF_SHININESS,
    CAMERA_PERSPECTIVE,
    CAMERA_LOOKAT,
    CAMERA_ORIENTATION,
    MODEL_VIEW_MATRIX,
    MODEL_VIEW_PERSPECTIVE_MATRIX,
    MODEL_VIEW_PERSPECTIVE_BIAS_MATRIX,
    NORMAL_MATRIX,
    LIGHT_VIEW_MATRIX,
};

#endif