
#include "olcConsoleGameEngine.h"
#include <iostream>

struct Vector3D
{
	float x;
	float y;
	float z;
};

struct Triangle
{
	Vector3D point[3];
};

struct Mesh3D
{
	vector<Triangle> Triangles;
};

struct matrix4x4
{
	float matrix[4][4] = { 0 };
};

class GraphicsEngine3D : public olcConsoleGameEngine
{
public:
	GraphicsEngine3D()
	{
		m_sAppName = L"3D_Renderer";
	}


private:
	Mesh3D meshCube;
	matrix4x4 projectionMatrix;
	float thetaValue;

	
	void MultiplyMatrix(Vector3D& vector1, Vector3D& vector2, matrix4x4& m)
	{
		vector2.x = vector1.x * m.matrix[0][0] + vector1.y * m.matrix[1][0] + vector1.z * m.matrix[2][0] + m.matrix[3][0];
		vector2.y = vector1.x * m.matrix[0][1] + vector1.y * m.matrix[1][1] + vector1.z * m.matrix[2][1] + m.matrix[3][1];
		vector2.z = vector1.x * m.matrix[0][2] + vector1.y * m.matrix[1][2] + vector1.z * m.matrix[2][2] + m.matrix[3][2];
		float w =   vector1.x * m.matrix[0][3] + vector1.y * m.matrix[1][3] + vector1.z * m.matrix[2][3] + m.matrix[3][3];

		if(w != 0.0f)
		{
			vector2.x /= w;
			vector2.y /= w;
			vector2.z /= w;
		}
	}

	public:
	bool OnUserCreate() override
	{
		meshCube.Triangles = {
			/*
			// Front
			{0.0f, 0.0f, 0.0f,       0.0f, 1.0f, 0.0f,     1.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f,       1.0f, 1.0f, 0.0f,     1.0f, 0.0f, 0.0f},
			
			// Right
			{1.0f, 0.0f, 0.0f,       1.0f, 1.0f, 0.0f,     1.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 0.0f,       1.0f, 1.0f, 1.0f,     1.0f, 0.0f, 1.0f},
			
			// Back
			{1.0f, 0.0f, 1.0f,       1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 1.0f,       0.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f},
			
			// Left
			{0.0f, 0.0f, 1.0f,       0.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f,       0.0f, 1.0f, 0.0f,     0.0f, 0.0f, 0.0f},
			// Top
			{0.0f, 1.0f, 0.0f,       0.0f, 1.0f, 1.0f,     1.0f, 1.0f, 1.0f},
			{0.0f, 1.0f, 0.0f,       1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 0.0f},
			// Bottom
			{1.0f, 0.0f, 1.0f,       0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 1.0f,       0.0f, 0.0f, 0.0f,     1.0f, 0.0f, 0.0f}
			*/
			// SOUTH
		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		// EAST                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		// NORTH                                                     
		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		// WEST                                                      
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		// TOP                                                       
		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		// BOTTOM                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
			
		};

		// Projection 2D into 3D Matrix
		const float nearPlane = 0.1f;
		const float farPlane = 1000.0f;
		const float fieldOfView = 90.0f;
		const float aspectRation = (float)(ScreenHeight()) / (float)(ScreenWidth());
		const float fieldOfViewRadians = 1.0f / tanf(fieldOfView * 0.5f / 180.0f * 3.14159f);

		projectionMatrix.matrix[0][0] = aspectRation * fieldOfViewRadians;
		projectionMatrix.matrix[1][1] = fieldOfViewRadians;
		projectionMatrix.matrix[2][2] = farPlane / (farPlane - nearPlane);
		projectionMatrix.matrix[3][2] = (-farPlane * nearPlane) / (farPlane - nearPlane);
		projectionMatrix.matrix[2][3] = 1.0f;
		projectionMatrix.matrix[3][3] = 0.0f;
		
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Fill(0, 0, ScreenHeight(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);


		matrix4x4 rotationalMatrixZAxis;
		matrix4x4 rotationalMatrixXAxis;
		thetaValue += 1.0f * fElapsedTime;

		// Rotation Z axis
		rotationalMatrixZAxis.matrix[0][0] = cosf(thetaValue);
		rotationalMatrixZAxis.matrix[0][1] = sinf(thetaValue);
		rotationalMatrixZAxis.matrix[1][0] = -sinf(thetaValue);
		rotationalMatrixZAxis.matrix[1][1] = cosf(thetaValue);
		rotationalMatrixZAxis.matrix[2][2] = 1;
		rotationalMatrixZAxis.matrix[3][3] = 1;

		// Rotation X axis
		rotationalMatrixXAxis.matrix[0][0] = 1;
		rotationalMatrixXAxis.matrix[1][1] = cosf(thetaValue * 0.5f);
		rotationalMatrixXAxis.matrix[1][2] = sinf(thetaValue * 0.5f);
		rotationalMatrixXAxis.matrix[2][1] = -sinf(thetaValue * 0.5f);
		rotationalMatrixXAxis.matrix[2][2] = cosf(thetaValue * 0.5f);
		rotationalMatrixXAxis.matrix[3][3] = 1;
		

		// Draw Triangles
		for(auto currentTriangle : meshCube.Triangles)
		{

			Triangle triangleProjected;
			Triangle triangleTranslated;
			Triangle triangleRotatedZ;
			Triangle triangleRotatedX;

			
			// Rotate original triangle about the z axis and then save into new triangle
			MultiplyMatrix(currentTriangle.point[0], triangleRotatedZ.point[0], rotationalMatrixZAxis);
			MultiplyMatrix(currentTriangle.point[1], triangleRotatedZ.point[1], rotationalMatrixZAxis);
			MultiplyMatrix(currentTriangle.point[2], triangleRotatedZ.point[2], rotationalMatrixZAxis);
			
			// Rotate z rotated triangle about the x axis and then save into new triangle
			MultiplyMatrix(triangleRotatedZ.point[0], triangleRotatedX.point[0], rotationalMatrixXAxis);
			MultiplyMatrix(triangleRotatedZ.point[1], triangleRotatedX.point[1], rotationalMatrixXAxis);
			MultiplyMatrix(triangleRotatedZ.point[2], triangleRotatedX.point[2], rotationalMatrixXAxis);
			

			// Translate triangle int the z-axis
			triangleTranslated = triangleRotatedX;
			triangleTranslated.point[0].z = triangleRotatedX.point[0].z + 3.0f;
			triangleTranslated.point[1].z = triangleRotatedX.point[1].z + 3.0f;
			triangleTranslated.point[2].z = triangleRotatedX.point[2].z + 3.0f;

			MultiplyMatrix(triangleTranslated.point[0], triangleProjected.point[0], projectionMatrix);
			MultiplyMatrix(triangleTranslated.point[1], triangleProjected.point[1], projectionMatrix);
			MultiplyMatrix(triangleTranslated.point[2], triangleProjected.point[2], projectionMatrix);

			// Scale into view:
			triangleProjected.point[0].x += 1.0f; triangleProjected.point[0].y += 1.0f;
			triangleProjected.point[1].x += 1.0f; triangleProjected.point[1].y += 1.0f;
			triangleProjected.point[2].x += 1.0f; triangleProjected.point[2].y += 1.0f;

			triangleProjected.point[0].x *= 0.5f * (float)ScreenWidth(); triangleProjected.point[0].y *= 0.5f * (float)ScreenHeight();
			triangleProjected.point[1].x *= 0.5f * (float)ScreenWidth(); triangleProjected.point[1].y *= 0.5f * (float)ScreenHeight();
			triangleProjected.point[2].x *= 0.5f * (float)ScreenWidth(); triangleProjected.point[2].y *= 0.5f * (float)ScreenHeight();

			

			
			DrawTriangle(triangleProjected.point[0].x, triangleProjected.point[0].y, triangleProjected.point[1].x,
			             triangleProjected.point[1].y, triangleProjected.point[2].x, triangleProjected.point[2].y,
			             PIXEL_SOLID, FG_WHITE);
			
		}

		
		return true;
	}
};



int main()
{
	GraphicsEngine3D rendererView;
	if (rendererView.ConstructConsole(300, 300, 4, 4))
		rendererView.Start();
	
    std::cout << "Hello World!\n";
}

