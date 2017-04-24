//  ---------------------------------------------------------------------------
//
//  @file       TwSimpleGLUT.c
//  @brief      A simple example that uses AntTweakBar with OpenGL and GLUT.
//
//              AntTweakBar: http://anttweakbar.sourceforge.net/doc
//              OpenGL:      http://www.opengl.org
//              GLUT:        http://opengl.org/resources/libraries/glut
//  
//  @author     Philippe Decaudin
//  @date       2006/05/20
//
//  ---------------------------------------------------------------------------


#include <AntTweakBar.h>

#include <Windows.h>
#include <glut.h>
#include <glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// TWEAKABLE PARAMETERS
// Camera Position
float g_CamPosition[] = { 0.0f, 0.0f, 5.0f };

// Camera Translation
float g_CamTranslation[] = { 0.0f, 0.0f, 0.0f };

// Camera Rotation
float g_CamRotation[] = { 0.0f, 0.0f, 0.0f };

// Clipping Plane
float g_ZFar = 100.0f;
float g_ZNear = 1.0f;

// Field of View
float g_FOVX = 0.0f;
float g_FOVY = 0.0f;

// Material Color
//float g_MatAmbient[] = { 0.5f, 0.0f, 0.0f, 1.0f };
//float g_MatDiffuse[] = { 1.0f, 1.0f, 0.0f, 1.0f };

// Booleans
int g_LookAtObject = 1;
int g_PerformBFCulling = 0;

// Options
// This example displays one of the following shapes
typedef enum { SHADING_POINTS = 1, SHADING_WIRE, SHADING_SOLID } Shading;
#define NUM_SHADING 3
Shading g_CurrentShading = SHADING_SOLID;

char* filename;


// This example displays one of the following shapes
typedef enum { SHAPE_TEAPOT = 1, SHAPE_TORUS, SHAPE_CONE } Shape;
#define NUM_SHAPES 3
Shape g_CurrentShape = SHAPE_TORUS;
// Shapes scale
float g_Zoom = 1.0f;
// Shape orientation (stored as a quaternion)
float g_Rotation[] = { 0.0f, 0.0f, 0.0f, 1.0f };
// Auto rotate
int g_AutoRotate = 0;
int g_RotateTime = 0;
float g_RotateStart[] = { 0.0f, 0.0f, 0.0f, 1.0f };
// Shapes material
float g_MatAmbient[] = { 0.5f, 0.0f, 0.0f, 1.0f };
float g_MatDiffuse[] = { 1.0f, 1.0f, 0.0f, 1.0f };
// Light parameter
float g_LightMultiplier = 1.0f;
float g_LightDirection[] = { -0.57735f, -0.57735f, -0.57735f };

// Return elapsed time in milliseconds
int GetTimeMs()
{
#if !defined(_WIN32)
	return glutGet(GLUT_ELAPSED_TIME);
#else
	// glutGet(GLUT_ELAPSED_TIME) seems buggy on Windows
	return (int)GetTickCount();
#endif
}


// Callback function called by GLUT to render screen
void Display(void)
{
	float v[4]; // will be used to set light parameters
	float mat[4 * 4]; // rotation matrix

	// Clear frame buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (g_LookAtObject)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(g_CamPosition[0] + g_CamTranslation[0], g_CamPosition[1] + g_CamTranslation[1],
			g_CamPosition[2] + g_CamTranslation[2], 0, 0, 0, 0, 1, 0);
	}
	else
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(g_CamPosition[0] + g_CamTranslation[0], g_CamPosition[1] + g_CamTranslation[1],
			g_CamPosition[2] + g_CamTranslation[2], g_CamTranslation[0], g_CamTranslation[1], g_CamTranslation[2], 0, 1, 0);
	}

	


	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);

	// Set light
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	v[0] = v[1] = v[2] = g_LightMultiplier*0.4f; v[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_AMBIENT, v);
	v[0] = v[1] = v[2] = g_LightMultiplier*0.8f; v[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_DIFFUSE, v);
	v[0] = -g_LightDirection[0]; v[1] = -g_LightDirection[1]; v[2] = -g_LightDirection[2]; v[3] = 0.0f;
	glLightfv(GL_LIGHT0, GL_POSITION, v);

	// Set material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, g_MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, g_MatDiffuse);

	// Rotate and draw shape
	glPushMatrix();
	//glTranslatef(0.5f, -0.3f, 0.0f);
	/*if (g_AutoRotate)
	{
		float axis[3] = { 0, 1, 0 };
		float angle = (float)(GetTimeMs() - g_RotateTime) / 1000.0f;
		float quat[4];
		SetQuaternionFromAxisAngle(axis, angle, quat);
		MultiplyQuaternions(g_RotateStart, quat, g_Rotation);
	}
	ConvertQuaternionToMatrix(g_Rotation, mat);*/
	//glMultMatrixf(mat);
	glScalef(g_Zoom, g_Zoom, g_Zoom);
	glutSolidTeapot(1.0);
	glPopMatrix();

	// Draw tweak bars
	TwDraw();

	// Present frame buffer
	glutSwapBuffers();

	// Recall Display at next frame
	glutPostRedisplay();
}


// Callback function called by GLUT when window size changes
void Reshape(int width, int height)
{
	// Set OpenGL viewport and camera
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)width / height, 1, 10);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//gluLookAt(g_CamPosition[0], g_CamPosition[1], g_CamPosition[2], 0, 0, 0, 0, 1, 0);
	glTranslatef(0, 0.6f, -1);

	// Send the new window size to AntTweakBar
	TwWindowSize(width, height);
}


// Function called at exit
void Terminate(void)
{
	glDeleteLists(SHAPE_TEAPOT, NUM_SHAPES);

	TwTerminate();
}


//  Callback function called when the 'AutoRotate' variable value of the tweak bar has changed
void TW_CALL SetAutoRotateCB(const void *value, void *clientData)
{
	(void)clientData; // unused

	g_AutoRotate = *(const int *)value; // copy value to g_AutoRotate
	if (g_AutoRotate != 0)
	{
		// init rotation
		g_RotateTime = GetTimeMs();
		g_RotateStart[0] = g_Rotation[0];
		g_RotateStart[1] = g_Rotation[1];
		g_RotateStart[2] = g_Rotation[2];
		g_RotateStart[3] = g_Rotation[3];

		// make Rotation variable read-only
		TwDefine(" TweakBar/ObjRotation readonly ");
	}
	else
		// make Rotation variable read-write
		TwDefine(" TweakBar/ObjRotation readwrite ");
}


//  Callback function called by the tweak bar to get the 'AutoRotate' value
void TW_CALL GetAutoRotateCB(void *value, void *clientData)
{
	(void)clientData; // unused
	*(int *)value = g_AutoRotate; // copy g_AutoRotate to value
}


// Main
int main(int argc, char *argv[])
{
	TwBar *bar; // Pointer to the tweak bar
	float axis[] = { 0.7f, 0.7f, 0.0f }; // initial model rotation
	float angle = 0.8f;

	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("Programming Assignment 1");
	glutCreateMenu(NULL);

	// Set GLUT callbacks
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	atexit(Terminate);  // Called after glutMainLoop ends

	// Initialize AntTweakBar
	TwInit(TW_OPENGL, NULL);

	// Set GLUT event callbacks
	// - Directly redirect GLUT mouse button events to AntTweakBar
	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	// - Directly redirect GLUT mouse motion events to AntTweakBar
	glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	// - Directly redirect GLUT mouse "passive" motion events to AntTweakBar (same as MouseMotion)
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	// - Directly redirect GLUT key events to AntTweakBar
	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
	// - Directly redirect GLUT special key events to AntTweakBar
	glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
	// - Send 'glutGetModifers' function pointer to AntTweakBar;
	//   required because the GLUT key event functions do not report key modifiers states.
	TwGLUTModifiersFunc(glutGetModifiers);

	// Create some 3D objects (stored in display lists)
	glNewList(SHAPE_TEAPOT, GL_COMPILE);
	glutSolidTeapot(1.0);
	glEndList();
	glNewList(SHAPE_TORUS, GL_COMPILE);
	glutSolidTorus(0.3, 1.0, 16, 32);
	glEndList();
	glNewList(SHAPE_CONE, GL_COMPILE);
	glutSolidCone(1.0, 1.5, 64, 4);
	glEndList();

	// Create a tweak bar
	bar = TwNewBar("Assignment 1");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLUT and OpenGL.' "); // Message added to the help bar.
	TwDefine(" TweakBar size='200 700' color='96 216 224' "); // change default tweak bar size and color

	//cam position
	TwAddVarRW(bar, "CamPosition", TW_TYPE_DIR3F, &g_CamPosition,
		" label='Camera Position' opened=false help='Change the camera position.' ");

	// cam translation
	TwAddVarRW(bar, "CamTranslation", TW_TYPE_DIR3F, &g_CamTranslation,
		" label='Camera Translation' opened=true help='Translate camera along u v or n.' ");
	
	// cam rotation
	TwAddVarRW(bar, "CamRotation", TW_TYPE_DIR3F, &g_CamRotation,
		" label='Camera Rotation' opened=true help='Rotate camera along u v or n.' ");

	// Look at object?
	TwAddVarCB(bar, "LookAtObject", TW_TYPE_BOOL32, SetAutoRotateCB, GetAutoRotateCB, NULL,
		" label='Look at object' key=space help='Toggle look at object.' ");
	
	//separates cam variables from the rest
	TwAddSeparator(bar, "sep1", NULL);
	TwDefine(" barName/sep1 group='Camera' ");

	// clipping plane
	// z far
	TwAddVarRW(bar, "Z Far", TW_TYPE_FLOAT, &g_ZFar,
		" min=0.0 max=100.0 step=0.5 keyIncr=f keyDecr=F help='Rotate camera along z axis ' ");

	// z near
	TwAddVarRW(bar, "Z Near", TW_TYPE_FLOAT, &g_ZNear,
		" min=0.0 max=100.0 step=0.5 keyIncr=n keyDecr=N help='Rotate camera along z axis ' ");

	// fov x
	TwAddVarRW(bar, "FOV x", TW_TYPE_FLOAT, &g_FOVX,
		" min=0.0 max=100.0 step=0.5 keyIncr=f keyDecr=F help='Rotate camera along z axis ' ");

	// fov y
	TwAddVarRW(bar, "FOV y", TW_TYPE_FLOAT, &g_FOVY,
		" min=0.0 max=100.0 step=0.5 keyIncr=n keyDecr=N help='Rotate camera along z axis ' ");

	//separates viewing variables from the rest
	TwAddSeparator(bar, "sep2", NULL);
	TwDefine(" barName/sep2 group='Viewing' ");

	// object/model variables

	// shading mode
	// (before adding an enum variable, its enum type must be declared to AntTweakBar as follow)
	{
		// ShapeEV associates Shape enum values with labels that will be displayed instead of enum values
		TwEnumVal shadingEV[NUM_SHADING] = { { SHADING_POINTS, "Points" }, { SHADING_WIRE, "Wire" }, { SHADING_SOLID, "Solid" } };
		// Create a type for the enum shapeEV
		TwType shadingType = TwDefineEnum("ShapeType", shadingEV, NUM_SHADING);
		// add 'g_CurrentShape' to 'bar': this is a variable of type ShapeType. Its key shortcuts are [<] and [>].
		TwAddVarRW(bar, "Shading", shadingType, &g_CurrentShading, " keyIncr='<' keyDecr='>' help='Change object shape.' ");
	}

	// Back Face Culling?
	TwAddVarCB(bar, "BFCulling", TW_TYPE_BOOL32, SetAutoRotateCB, GetAutoRotateCB, NULL,
		" label='Back Face Culling' key=enter help='Toggle back face culling.' ");

	// Add 'g_MatAmbient' to 'bar': this is a variable of type TW_TYPE_COLOR3F (3 floats color, alpha is ignored)
	// and is inserted into a group named 'Material'.
	TwAddVarRW(bar, "Ambient", TW_TYPE_COLOR3F, &g_MatAmbient, " group='Material' ");

	// Add 'g_MatDiffuse' to 'bar': this is a variable of type TW_TYPE_COLOR3F (3 floats color, alpha is ignored)
	// and is inserted into group 'Material'.
	TwAddVarRW(bar, "Diffuse", TW_TYPE_COLOR3F, &g_MatDiffuse, " group='Material' ");

	// load .obj file


	// Store time
	g_RotateTime = GetTimeMs();
	// Init rotation
	//SetQuaternionFromAxisAngle(axis, angle, g_Rotation);
	//SetQuaternionFromAxisAngle(axis, angle, g_RotateStart);

	// Call the GLUT main loop
	glutMainLoop();

	return 0;
}

