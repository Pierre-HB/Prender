#include "Matrix4.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

std::ostream& operator<<(std::ostream& o, const mat4& m) {
	o << "[" << m.c[0] << ", " << m.c[1] << ", " << m.c[2] << ", " << m.c[3] << " | " << m.c[4] << ", " << m.c[5] << ", " << m.c[6] << ", " << m.c[7] << " | " << m.c[8] << ", " << m.c[9] << ", " << m.c[10] << ", " << m.c[11] << " | " << m.c[12] << ", " << m.c[13] << ", " << m.c[14] << ", " << m.c[15] << "]";
	return o;
}

mat4* mat4::operator+=(const mat4& m) {
	for (int i = 0; i < 16; i++)
		c[i] += m.c[i];
	return this;
}

mat4* mat4::operator-=(const mat4& m) {
	for (int i = 0; i < 16; i++)
		c[i] -= m.c[i];
	return this;
}

mat4* mat4::operator*=(const mat4& m)
{
	float tmp[16]{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	/*
	| tmp[0 ]  tmp[1 ]  tmp[2 ]  tmp[3 ]
	| tmp[4 ]  tmp[5 ]  tmp[6 ]  tmp[7 ]
	| tmp[8 ]  tmp[9 ]  tmp[10]  tmp[11]
	| tmp[12]  tmp[13]  tmp[14]  tmp[15]
	*/
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				tmp[4 * i + j] += c[4 * i + k] * m.c[4 * k + j];
	for (int i = 0; i < 16; i++)
		c[i] = tmp[i];
	return this;
}

mat4* mat4::operator*=(float s) {
	for (int i = 0; i < 16; i++)
		c[i] *= s;
	return this;
}

mat4* mat4::operator/=(float s) {
	for (int i = 0; i < 16; i++)
		c[i] /= s;
	return this;
}

vec3 mat4::extractTranslation() const {
    /*
    * Assuming that the matrix as the form:
    |a b c Tx
    |d e f Ty
    |g h i Tz
    |0 0 0 1
    */
    return vec3(c[3], c[7], c[11]);
}

mat4 mat4::extractRotation() const {
    /*
    * Assuming that the matrix as the form:
    |a b c Tx
    |d e f Ty
    |g h i Tz
    |0 0 0 1
    AND a determinant of 1
    return :
    |a b c 0
    |d e f 0
    |g h i 0
    |0 0 0 1
    */
    return mat4(c[0], c[1], c[2],  0,
                c[4], c[5], c[6],  0,
                c[8], c[9], c[10], 0,
                0,    0,    0,     1);
}

vec3 mat4::extractScale() const {
    float sx = sqrt(c[0] * c[0] + c[4] * c[4] + c[8] * c[8]);
    float sy = sqrt(c[1] * c[1] + c[5] * c[5] + c[9] * c[9]);
    float sz = sqrt(c[2] * c[2] + c[6] * c[6] + c[10] * c[10]);
    return vec3(sx, sy, sz);
}


mat4 operator+(const mat4& a, const mat4& b) {
	mat4 tmp = mat4();
	for (int i = 0; i < 16; i++)
		tmp.c[i] = a.c[i] + b.c[i];
	return tmp;
}

mat4 operator-(const mat4& a, const mat4& b) {
	mat4 tmp = mat4();
	for (int i = 0; i < 16; i++)
		tmp.c[i] = a.c[i] - b.c[i];
	return tmp;
}

mat4 operator*(const mat4& a, const mat4& b) {
	mat4 tmp = mat4();
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				tmp.c[4 * i + j] += a.c[4 * i + k] * b.c[4 * k + j];
	return tmp;
}

vec4 operator*(const mat4& m, const vec4& v) {
	vec4 tmp = vec4();

	tmp.x = m.c[0] * v.x + m.c[1] * v.y + m.c[2] * v.z + m.c[3] * v.w;
	tmp.y = m.c[4] * v.x + m.c[5] * v.y + m.c[6] * v.z + m.c[7] * v.w;
	tmp.z = m.c[8] * v.x + m.c[9] * v.y + m.c[10] * v.z + m.c[11] * v.w;
	tmp.w = m.c[12] * v.x + m.c[13] * v.y + m.c[14] * v.z + m.c[15] * v.w;

	return tmp;
}

vec3 operator*(const mat4& m, const vec3& v) {
	vec3 tmp = vec3();

	tmp.x = m.c[0] * v.x + m.c[1] * v.y + m.c[2] * v.z + m.c[3];
	tmp.y = m.c[4] * v.x + m.c[5] * v.y + m.c[6] * v.z + m.c[7];
	tmp.z = m.c[8] * v.x + m.c[9] * v.y + m.c[10] * v.z + m.c[11];
	float w = m.c[12] * v.x + m.c[13] * v.y + m.c[14] * v.z + m.c[15];

	return tmp / w;
}

mat4 operator*(const mat4& m, float s) {
	mat4 tmp = mat4();
	for (int i = 0; i < 16; i++)
		tmp.c[i] = m.c[i] * s;
	return tmp;
}

mat4 operator*(float s, const mat4& m) {
	return m * s;
}

mat4 operator/(const mat4& m, float s) {
	mat4 tmp = mat4();
	for (int i = 0; i < 16; i++)
		tmp.c[i] = m.c[i] / s;
	return tmp;
}

mat4 transpose(const mat4& m) {
	mat4 tmp = mat4();
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			tmp.c[4 * i + j] = m.c[4 * j + i];
	return tmp;
}

mat4 inverse(const mat4& m) {

    mat4 tmp = mat4();
    tmp.c[0] = m.c[5] * m.c[10] * m.c[15] -
        m.c[5] * m.c[11] * m.c[14] -
        m.c[9] * m.c[6] * m.c[15] +
        m.c[9] * m.c[7] * m.c[14] +
        m.c[13] * m.c[6] * m.c[11] -
        m.c[13] * m.c[7] * m.c[10];

    tmp.c[4] = -m.c[4] * m.c[10] * m.c[15] +
        m.c[4] * m.c[11] * m.c[14] +
        m.c[8] * m.c[6] * m.c[15] -
        m.c[8] * m.c[7] * m.c[14] -
        m.c[12] * m.c[6] * m.c[11] +
        m.c[12] * m.c[7] * m.c[10];

    tmp.c[8] = m.c[4] * m.c[9] * m.c[15] -
        m.c[4] * m.c[11] * m.c[13] -
        m.c[8] * m.c[5] * m.c[15] +
        m.c[8] * m.c[7] * m.c[13] +
        m.c[12] * m.c[5] * m.c[11] -
        m.c[12] * m.c[7] * m.c[9];

    tmp.c[12] = -m.c[4] * m.c[9] * m.c[14] +
        m.c[4] * m.c[10] * m.c[13] +
        m.c[8] * m.c[5] * m.c[14] -
        m.c[8] * m.c[6] * m.c[13] -
        m.c[12] * m.c[5] * m.c[10] +
        m.c[12] * m.c[6] * m.c[9];

    tmp.c[1] = -m.c[1] * m.c[10] * m.c[15] +
        m.c[1] * m.c[11] * m.c[14] +
        m.c[9] * m.c[2] * m.c[15] -
        m.c[9] * m.c[3] * m.c[14] -
        m.c[13] * m.c[2] * m.c[11] +
        m.c[13] * m.c[3] * m.c[10];

    tmp.c[5] = m.c[0] * m.c[10] * m.c[15] -
        m.c[0] * m.c[11] * m.c[14] -
        m.c[8] * m.c[2] * m.c[15] +
        m.c[8] * m.c[3] * m.c[14] +
        m.c[12] * m.c[2] * m.c[11] -
        m.c[12] * m.c[3] * m.c[10];

    tmp.c[9] = -m.c[0] * m.c[9] * m.c[15] +
        m.c[0] * m.c[11] * m.c[13] +
        m.c[8] * m.c[1] * m.c[15] -
        m.c[8] * m.c[3] * m.c[13] -
        m.c[12] * m.c[1] * m.c[11] +
        m.c[12] * m.c[3] * m.c[9];

    tmp.c[13] = m.c[0] * m.c[9] * m.c[14] -
        m.c[0] * m.c[10] * m.c[13] -
        m.c[8] * m.c[1] * m.c[14] +
        m.c[8] * m.c[2] * m.c[13] +
        m.c[12] * m.c[1] * m.c[10] -
        m.c[12] * m.c[2] * m.c[9];

    tmp.c[2] = m.c[1] * m.c[6] * m.c[15] -
        m.c[1] * m.c[7] * m.c[14] -
        m.c[5] * m.c[2] * m.c[15] +
        m.c[5] * m.c[3] * m.c[14] +
        m.c[13] * m.c[2] * m.c[7] -
        m.c[13] * m.c[3] * m.c[6];

    tmp.c[6] = -m.c[0] * m.c[6] * m.c[15] +
        m.c[0] * m.c[7] * m.c[14] +
        m.c[4] * m.c[2] * m.c[15] -
        m.c[4] * m.c[3] * m.c[14] -
        m.c[12] * m.c[2] * m.c[7] +
        m.c[12] * m.c[3] * m.c[6];

    tmp.c[10] = m.c[0] * m.c[5] * m.c[15] -
        m.c[0] * m.c[7] * m.c[13] -
        m.c[4] * m.c[1] * m.c[15] +
        m.c[4] * m.c[3] * m.c[13] +
        m.c[12] * m.c[1] * m.c[7] -
        m.c[12] * m.c[3] * m.c[5];

    tmp.c[14] = -m.c[0] * m.c[5] * m.c[14] +
        m.c[0] * m.c[6] * m.c[13] +
        m.c[4] * m.c[1] * m.c[14] -
        m.c[4] * m.c[2] * m.c[13] -
        m.c[12] * m.c[1] * m.c[6] +
        m.c[12] * m.c[2] * m.c[5];

    tmp.c[3] = -m.c[1] * m.c[6] * m.c[11] +
        m.c[1] * m.c[7] * m.c[10] +
        m.c[5] * m.c[2] * m.c[11] -
        m.c[5] * m.c[3] * m.c[10] -
        m.c[9] * m.c[2] * m.c[7] +
        m.c[9] * m.c[3] * m.c[6];

    tmp.c[7] = m.c[0] * m.c[6] * m.c[11] -
        m.c[0] * m.c[7] * m.c[10] -
        m.c[4] * m.c[2] * m.c[11] +
        m.c[4] * m.c[3] * m.c[10] +
        m.c[8] * m.c[2] * m.c[7] -
        m.c[8] * m.c[3] * m.c[6];

    tmp.c[11] = -m.c[0] * m.c[5] * m.c[11] +
        m.c[0] * m.c[7] * m.c[9] +
        m.c[4] * m.c[1] * m.c[11] -
        m.c[4] * m.c[3] * m.c[9] -
        m.c[8] * m.c[1] * m.c[7] +
        m.c[8] * m.c[3] * m.c[5];

    tmp.c[15] = m.c[0] * m.c[5] * m.c[10] -
        m.c[0] * m.c[6] * m.c[9] -
        m.c[4] * m.c[1] * m.c[10] +
        m.c[4] * m.c[2] * m.c[9] +
        m.c[8] * m.c[1] * m.c[6] -
        m.c[8] * m.c[2] * m.c[5];

    float invdet = 1/(m.c[0] * tmp.c[0] +
        m.c[1] * tmp.c[4] +
        m.c[2] * tmp.c[8] +
        m.c[3] * tmp.c[12]);

    return tmp * invdet;
}

mat4 rotationMatrixX(float a) {
    return mat4(1, 0, 0, 0,
                0, cos(a), -sin(a), 0,
                0, sin(a), cos(a), 0,
                0, 0, 0, 1);
}

mat4 rotationMatrixY(float a) {
    return mat4(cos(a),     0,   sin(a),     0,
                0,          1,  0,          0,
                -sin(a),    0,  cos(a),     0,
                0,          0,  0,          1);
}

mat4 rotationMatrixZ(float a) {
    return mat4(cos(a), -sin(a), 0, 0,
                sin(a), cos(a), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1);
}

mat4 translationMatrix(const vec3& v) {
    return mat4(1, 0, 0, v.x,
                0, 1, 0, v.y,
                0, 0, 1, v.z,
                0, 0, 0, 1);
}

mat4 scaleMatrix(float s) {
    return mat4(s, 0, 0, 0,
                0, s, 0, 0,
                0, 0, s, 0,
                0, 0, 0, 1);
}

mat4 scaleMatrix(const vec3& scales) {
    return mat4(scales.x, 0, 0, 0,
                0, scales.y, 0, 0,
                0, 0, scales.z, 0,
                0, 0, 0, 1);
}

mat4 normalTransformation(const mat4& m) {
    return transpose(inverse(m));
}

mat4 transformationMatrix(const vec3& rotations, const vec3& scales, const vec3& translation) {
    return rotationMatrixZ(rotations.z) * rotationMatrixY(rotations.y) * rotationMatrixX(rotations.x) * scaleMatrix(scales) * translationMatrix(translation);
}

#ifdef IMGUI

ImGuiTransformationAttr::ImGuiTransformationAttr(const mat4& transformation) : name("Transformation") {
    extract_transormations(transformation);
    rotations = vec3();
}

ImGuiTransformationAttr::ImGuiTransformationAttr(const mat4& transformation, const char* name) : name(name) {
    extract_transormations(transformation);
    rotations = vec3();
}

void ImGuiTransformationAttr::extract_transormations(const mat4& transformation) {
    translation = -inverse(transformation).extractTranslation();
    OriginalRotation = transformation * translationMatrix(-translation);
    scales = OriginalRotation.extractScale();
    OriginalRotation = OriginalRotation * scaleMatrix(vec3(1 / scales.x, 1 / scales.y, 1 / scales.z));
    OriginalRotation = OriginalRotation.extractRotation(); //for stability
}

void imGuiPrintAttribute(ImGuiTransformationAttr* transformAttr){
    if (ImGui::TreeNode(transformAttr->name)) {
        ImGui::PushItemWidth(75 * 3);
        ImGui::DragFloat3(": translation", &(transformAttr->translation.x));
        ImGui::PopItemWidth();

        ImGui::PushItemWidth(75*3);
        ImGui::DragFloat3(": scales", &(transformAttr->scales.x), 1, 1e-3, 1e10);
        ImGui::PopItemWidth();

        float x, y, z;
        x = transformAttr->rotations.x;
        y = transformAttr->rotations.y;
        z = transformAttr->rotations.z;
        ImGui::Text("Rotation :");

        ImGui::Columns(2, "Rotation");

        ImGui::PushItemWidth(75);
        ImGui::DragFloat(" : x", &x);
        ImGui::PopItemWidth();

        ImGui::PushItemWidth(75);
        ImGui::DragFloat(" : y", &y);
        ImGui::PopItemWidth();

        ImGui::PushItemWidth(75);
        ImGui::DragFloat(" : z", &z);
        ImGui::PopItemWidth();

        ImGui::NextColumn();

        static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit;

        if (ImGui::BeginTable("Rot Matrix", 4, flags))
        {
            for (int i = 0; i < 4; i++) {
                ImGui::TableNextRow();
                for (int j = 0; j < 4; j++) {
                    ImGui::TableSetColumnIndex(j);
                    ImGui::Text("%1.2f", transformAttr->OriginalRotation.c[i * 4 + j]);
                }
            }
            ImGui::EndTable();
        }

        ImGui::Columns(1);

        float dx = x - transformAttr->rotations.x;
        float dy = y - transformAttr->rotations.y;
        float dz = z - transformAttr->rotations.z;

        if (dx != 0.0f) {
            transformAttr->rotations.x = x;
            transformAttr->rotations.y = 0;
            transformAttr->rotations.z = 0;
            transformAttr->OriginalRotation = rotationMatrixX(dx * 2 * M_PI / 360) * transformAttr->OriginalRotation;
        }
        if (dy != 0.0f) {
            transformAttr->rotations.x = 0;
            transformAttr->rotations.y = y;
            transformAttr->rotations.z = 0;
            transformAttr->OriginalRotation = rotationMatrixY(dy * 2 * M_PI / 360) * transformAttr->OriginalRotation;
        }
        if (dz != 0.0f) {
            transformAttr->rotations.x = 0;
            transformAttr->rotations.y = 0;
            transformAttr->rotations.z = z;
            transformAttr->OriginalRotation = rotationMatrixZ(dz * 2 * M_PI / 360) * transformAttr->OriginalRotation;
        }

        ImGui::TreePop();
    }
}


void ImGuiTransformationAttr::updateAttr(const mat4& transformation) {
    mat4 inv = inverse(transformation);
    translation = -inv.extractTranslation();
    mat4 tmp = transformation * translationMatrix(-translation);
    scales = tmp.extractScale();
    tmp = tmp * scaleMatrix(vec3(1 / scales.x, 1 / scales.y, 1 / scales.z));
    tmp = tmp.extractRotation(); //for stability

    mat4 dRot = tmp * transpose(OriginalRotation);
    //tmp = Rot(Z+dZ)*Rot(Y+dY)*Rot(X+dX) * originalRotation
    //dRot = Rot(Z+dZ)*Rot(Y+dY)*Rot(X+dX)
    //dRot = Rot(Z+dZ)*Rot(Y+dY)*Rot(dX)*RotX
    // 
    //assume dX is near 0 i.e sin is bijectif and cos is > 0
    dRot = dRot * rotationMatrixX(-rotations.x);
    //dRot' = dRot*Rot(-X)
    //dRot' = Rot(Z+dZ)*Rot(Y+dY)*Rot(dX)*RotX*Rot(-X)
    //dRot' = Rot(Z+dZ)*Rot(Y+dY)*Rot(dX)
    //dRot'_1 = Rot(-dX)*Rot(-Y-dY)*Rot(-Z-dZ)
    //
    //for z = (0, 0, 1) eigenvector of Rot(-Z-dZ):
    //dRot'_1*z = Rot(-dX)*Rot(-Y-dY)*z
    //         = Rot(-dX)

    //Rot(Y)= |cos(Y)  0 sin(Y)
    //        |0       1     0
    //        |-sin(Y) 0 cos(Y)

    //dRot'_1*z = Rot(-dX)*(sin(-Y-dY), 0, cos(-Y-dY))

    //Rot(X)= |1 0      0
    //        |0 cos(X) -sin(X)
    //        |0 sin(X) cos(X)

    //dRot'_1*z = (sin(-Y-dY), -sin(-dX)*cos(-Y-dY), cos(-dX)*cos(-Y-dY))

    //sin(a+b) = sin(a)cos(b) + cos(a)sin(b)
    //cos(a+b) = cos(a)cos(b) - sin(a)sin(b)


    //dRot'' = Rot(-Z)*dRot*Rot(-X)
    //dRot'' = Rot(-Z)*Rot(Z+dZ)*Rot(Y+dY)*Rot(dX)*RotX*Rot(-X)
    //dRot'' = Rot(dZ)*Rot(Y+dY)*Rot(dX)

    //for y = (0, 1, 0)
    
    //Rot(X)= |1 0      0
    //        |0 cos(X) -sin(X)
    //        |0 sin(X) cos(X)

    //dRot''*y = Rot(dZ)*Rot(Y+dY)*(0, cos(dX), sin(dX))

    //Rot(Y)= |cos(Y)  0 sin(Y)
    //        |0       1     0
    //        |-sin(Y) 0 cos(Y)

    //dRot''*y = Rot(dZ)*(sin(Y+dY)sin(dX), cos(dX), cos(Y+dY)sin(dX))

    //Rot(Z)= |cos(Z) -sin(Z) 0
    //        |sin(Z) cos(Z)  0
    //        |0      0       1

    //dRot''*y = (cos(dZ)sin(Y+dY)sin(dX) -sin(dZ)cos(dX),
    //            sin(dZ)sin(Y+dY)sin(dX) +cos(dZ)cos(dX),
    //            cos(Y+dY)sin(dX) 



    //Rot(X)= |1 0      0
    //        |0 cos(X) -sin(X)
    //        |0 sin(X) cos(X)

    //Rot(Y)= |cos(Y)  0 sin(Y)
    //        |0       1     0
    //        |-sin(Y) 0 cos(Y)

    //Rot(Z)= |cos(Z) -sin(Z) 0
    //        |sin(Z) cos(Z)  0
    //        |0      0       1

    //Rot(Z)*Rot(Y)*Rot(X) = 
    //
    // Rot(Z)*
    // |cos(Y)      sin(Y)sin(X)        sin(Y)cos(X)
    // |0           cos(X)              -sin(X)
    // |-sin(Y)     sin(X)cos(Y)        cos(X)cos(Y)
    //
    // =
    //
    // |cos(Z)cos(Y)    
    // |
    // |

    //compute the two angles values from : https://eecs.qmul.ac.uk/~gslabaugh/publications/euler.pdf
    //choos the closest one to my problem
    //IF cos(Y)~ 0 <=> sin(Y)~1



    rotations = vec3();
}

#endif