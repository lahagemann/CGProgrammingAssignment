#include <AntTweakBar.h>

#include <Windows.h>
#include <glut.h>
#include <glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Model.h"

// TWEAKABLE PARAMETERS
// Camera Position
float g_CamPosition[] = { 0.0f, 0.0f, 5.0f };

// Camera Translation
float g_CamTranslation[] = { 0.0f, 0.0f, 0.0f };

// Camera Rotation
float g_CamRotation[] = { 0.0f, 0.0f, 0.0f };

// Clipping Plane
float g_ZFar = 2000.0f;
float g_ZNear = 1.0f;

// Field of View
float g_FOVX = 128.0f;
float g_FOVY = 72.0f;

// Material Color
float g_MatAmbient[] = { 0.5f, 0.0f, 0.0f, 1.0f };
float g_MatDiffuse[] = { 0.2f, 0.2f, 0.2f, 1.0f };

// Light parameter
float g_LightMultiplier = 1.0f;
float g_LightDirection[] = { 1.1547f, -1.1547f, 1.1547f };

// Booleans
int g_LookAtObject = 1;
int g_PerformBFCulling = 0;

// Options
// This example displays one of the following shapes
typedef enum { SHADING_POINTS = 1, SHADING_WIRE, SHADING_SOLID } Shading;
#define NUM_SHADING 3
Shading g_CurrentShading = SHADING_SOLID;

char filename[256] = "cow_up.in";

// other variables
int width = 1280;
int height = 720;
Model m;

// CAMERA AXIS
float u[] = { 0.0f, 0.0f, 0.0f };
float v[] = { 0.0f, 0.0f, 0.0f };
float n[] = { 0.0f, 0.0f, 0.0f };

// OBJECT CENTER IN WCS
float obj_center[] = { 0.0f, 0.0f, 0.0f };

float g_Zoom = 1.0f;



//Draw model
void Draw(Model m)
{
	if (g_CurrentShading == SHADING_SOLID) 
	{
		for (int i = 0; i < m.numTriangles; i++)
		{
			glColor4f(g_MatDiffuse[0], g_MatDiffuse[1], g_MatDiffuse[2], g_MatDiffuse[3]);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBegin(GL_TRIANGLES);
			glNormal3f(m.triangles[i].normal[0].x, m.triangles[i].normal[0].y, m.triangles[i].normal[0].z);
			glVertex3f(m.triangles[i].v0.x, m.triangles[i].v0.y, m.triangles[i].v0.z);
			glNormal3f(m.triangles[i].normal[1].x, m.triangles[i].normal[1].y, m.triangles[i].normal[1].z);
			glVertex3f(m.triangles[i].v1.x, m.triangles[i].v1.y, m.triangles[i].v1.z);
			glNormal3f(m.triangles[i].normal[2].x, m.triangles[i].normal[2].y, m.triangles[i].normal[2].z);
			glVertex3f(m.triangles[i].v2.x, m.triangles[i].v2.y, m.triangles[i].v2.z);			
			glEnd();
		}
	}
	else if (g_CurrentShading == SHADING_WIRE)
	{
		for (int i = 0; i < m.numTriangles; i++)
		{
			glColor4f(g_MatDiffuse[0], g_MatDiffuse[1], g_MatDiffuse[2], g_MatDiffuse[3]);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBegin(GL_TRIANGLES);
			glNormal3f(m.triangles[i].normal[0].x, m.triangles[i].normal[0].y, m.triangles[i].normal[0].z);
			glVertex3f(m.triangles[i].v0.x, m.triangles[i].v0.y, m.triangles[i].v0.z);
			glNormal3f(m.triangles[i].normal[1].x, m.triangles[i].normal[1].y, m.triangles[i].normal[1].z);
			glVertex3f(m.triangles[i].v1.x, m.triangles[i].v1.y, m.triangles[i].v1.z);
			glNormal3f(m.triangles[i].normal[2].x, m.triangles[i].normal[2].y, m.triangles[i].normal[2].z);
			glVertex3f(m.triangles[i].v2.x, m.triangles[i].v2.y, m.triangles[i].v2.z);
			glEnd();
		}
	}
	else 
	{
		for (int i = 0; i < m.numTriangles; i++)
		{
			glColor4f(g_MatDiffuse[0], g_MatDiffuse[1], g_MatDiffuse[2], g_MatDiffuse[3]);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBegin(GL_POINTS);
			glNormal3f(m.triangles[i].normal[0].x, m.triangles[i].normal[0].y, m.triangles[i].normal[0].z);
			glVertex3f(m.triangles[i].v0.x, m.triangles[i].v0.y, m.triangles[i].v0.z);
			glNormal3f(m.triangles[i].normal[1].x, m.triangles[i].normal[1].y, m.triangles[i].normal[1].z);
			glVertex3f(m.triangles[i].v1.x, m.triangles[i].v1.y, m.triangles[i].v1.z);
			glNormal3f(m.triangles[i].normal[2].x, m.triangles[i].normal[2].y, m.triangles[i].normal[2].z);
			glVertex3f(m.triangles[i].v2.x, m.triangles[i].v2.y, m.triangles[i].v2.z);
			glEnd();
		}
	}
	return;
}

void set_camera_position_auto()
{
	obj_center[0] = (m.max.x + m.min.x) / 2;
	obj_center[1] = (m.max.y + m.min.y) / 2;
	obj_center[2] = (m.max.z + m.min.z) / 2;

	//x = center x?
	//g_CamPosition[0] = c[0];
	// z = center x + half / 0.5774 if tan 30 -> no, z = center x + half  if tan 45 -> works better
	g_CamPosition[2] = (obj_center[0] + (m.max.x - m.min.x) / 2);

}


// Callback function called by GLUT to render screen
void Display(void)
{
	float v[4]; // will be used to set light parameters

	// Clear frame buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// viewport and frustum setup
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(g_FOVY, (double)g_FOVX / g_FOVY, g_ZNear, g_ZFar);

	// camera setup
	if (g_LookAtObject)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(g_CamPosition[0] + g_CamTranslation[0], g_CamPosition[1] + g_CamTranslation[1],
			g_CamPosition[2] + g_CamTranslation[2], obj_center[0], obj_center[1], obj_center[2], 0, 1, 0);
	}
	else
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(g_CamPosition[0] + g_CamTranslation[0], g_CamPosition[1] + g_CamTranslation[1], g_CamPosition[2] + g_CamTranslation[2], 
			obj_center[0] + g_CamTranslation[0], obj_center[1] + g_CamTranslation[1], obj_center[2] + g_CamTranslation[2], 
			0, 1, 0);
	}

	// culling setup (NOT WORKING?)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	if (g_PerformBFCulling) 
	{
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else
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
	glScalef(g_Zoom, g_Zoom, g_Zoom);
	//if (g_CurrentShading == SHADING_SOLID)
	//	glutSolidTeapot(1.0);
	//else
	//	glutWireTeapot(1.0);
	Draw(m);
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
	TwTerminate();
}

//  Callback function called when the 'LookAtObject' variable value of the tweak bar has changed
void TW_CALL SetLookAtObjectCB(const void *value, void *clientData)
{
	(void)clientData; // unused

	g_LookAtObject = *(const int *)value; // copy value to LookAtObject
}


//  Callback function called by the tweak bar to get the 'LookAtObject' value
void TW_CALL GetLookAtObjectCB(void *value, void *clientData)
{
	(void)clientData; // unused
	*(int *)value = g_LookAtObject; // copy LookAtObject to value
}

//  Callback function called when the 'BFCulling' variable value of the tweak bar has changed
void TW_CALL SetBFCullingCB(const void *value, void *clientData)
{
	(void)clientData; // unused

	g_PerformBFCulling = *(const int *)value; // copy value to g_PerformBFCulling
}


//  Callback function called by the tweak bar to get the 'BFCulling' value
void TW_CALL GetBFCullingCB(void *value, void *clientData)
{
	(void)clientData; // unused
	*(int *)value = g_PerformBFCulling; // copy g_PerformBFCulling to value
}

void TW_CALL ResetCam(void * /*clientData*/)
{
	set_camera_position_auto();

	g_CamTranslation[0] = 0.0f;
	g_CamTranslation[1] = 0.0f;
	g_CamTranslation[2] = 0.0f;

	g_CamRotation[0] = 0.0f;
	g_CamRotation[1] = 0.0f;
	g_CamRotation[2] = 0.0f;
}

void TW_CALL LoadModel(void * /*clientData*/)
{
	m = Model(filename);
	set_camera_position_auto();
}

// Main
int main(int argc, char *argv[])
{
	TwBar *bar; // Pointer to the tweak bar
	float axis[] = { 0.7f, 0.7f, 0.0f }; // initial model rotation
	float angle = 0.8f;

	m = Model(filename);
	set_camera_position_auto();

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
	TwAddVarCB(bar, "LookAtObject", TW_TYPE_BOOL32, SetLookAtObjectCB, GetLookAtObjectCB, NULL,
		" label='Look at object' key=space help='Toggle look at object.' ");
	// ...
	TwAddButton(bar, "Reset", ResetCam, NULL, " label='Reset' ");

	// clipping plane
	// z far
	TwAddVarRW(bar, "Z Far", TW_TYPE_FLOAT, &g_ZFar,
		" min=0.0 max=4000.0 step=0.5 keyIncr=f keyDecr=F help='Rotate camera along z axis ' ");

	// z near
	TwAddVarRW(bar, "Z Near", TW_TYPE_FLOAT, &g_ZNear,
		" min=0.0 max=100.0 step=0.5 keyIncr=n keyDecr=N help='Rotate camera along z axis ' ");

	// fov x
	TwAddVarRW(bar, "FOV x", TW_TYPE_FLOAT, &g_FOVX,
		" min=0.0 max=500.0 step=0.5 keyIncr=f keyDecr=F help='Rotate camera along z axis ' ");

	// fov y
	TwAddVarRW(bar, "FOV y", TW_TYPE_FLOAT, &g_FOVY,
		" min=0.0 max=500.0 step=0.5 keyIncr=n keyDecr=N help='Rotate camera along z axis ' ");

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
	TwAddVarCB(bar, "BFCulling", TW_TYPE_BOOL32, SetBFCullingCB, GetBFCullingCB, NULL,
		" label='Back Face Culling' key=enter help='Toggle back face culling.' ");

	// Add 'g_MatAmbient' to 'bar': this is a variable of type TW_TYPE_COLOR3F (3 floats color, alpha is ignored)
	// and is inserted into a group named 'Material'.
	TwAddVarRW(bar, "Ambient", TW_TYPE_COLOR3F, &g_MatAmbient, " group='Material' ");

	// Add 'g_MatDiffuse' to 'bar': this is a variable of type TW_TYPE_COLOR3F (3 floats color, alpha is ignored)
	// and is inserted into group 'Material'.
	TwAddVarRW(bar, "Diffuse", TW_TYPE_COLOR3F, &g_MatDiffuse, " group='Material' ");

	// load .obj file
	TwAddVarRW(bar, "Filename", TW_TYPE_CSSTRING(sizeof(filename)), filename, " label='file' group=CDString help='type file name.' ");

	// ...
	TwAddButton(bar, "Load", LoadModel, NULL, " label='Load Model' ");

	// Call the GLUT main loop
	glutMainLoop();

	return 0;
}

