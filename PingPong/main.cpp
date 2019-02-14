#define _USE_MATH_DEFINES
#include <Windows.h>
#include <GL/gl.h>
#include "glut.h"
#include <math.h>
#include <iostream>
#include <time.h>  
#include <vector>
#include <chrono>
#include <thread>
#include <functional>
using namespace std;

void DrawReferenceFrame();

//Decidi usar mi funcion transform, para trasladar la parte al pivote que quiero de mi pieza
//La implementacion de Draw tiene lo necesario para el escalamiento de la pieza

class Node {
public:
	Node **Child;
	int ChildCount;
	float **paramsT;
	float **paramsD;
	void(*Transform)(float **params);
	void(*Draw)(float **params);

	Node() {
		ChildCount = 0;
		Child = NULL;
		paramsT = NULL;
		paramsD = NULL;
	}

	static void DrawObj(Node *nodo) {
		glPushMatrix();
		if (nodo->Transform != NULL)
			nodo->Transform(nodo->paramsT);
		glPushMatrix();
		if (nodo->Draw != NULL)
			nodo->Draw(nodo->paramsD);
		glPopMatrix();
		for (int i = 0; i < nodo->ChildCount; i++)
			DrawObj(nodo->Child[i]);
		glPopMatrix();
	}
};

namespace RobotFunc {
	//Torzo
	void TrasnformTorzo(float **params) { glRotated(*params[0], 0, 1, 0); }
	void DrawTorzo(float **params) {
		glScaled(1, 2, 0.5);
		glTranslated(0, 0.5, 0);
		glutSolidCube(1);
	}

	//Head
	void TransformHead(float **params) { glTranslated(0, 2, 0); glRotated(*params[0], 1, 0, 0); }
	void DrawHead(float **params) {
		glTranslated(0, 0.35, 0);
		glScaled(0.4, 0.4, 0.4);
		glutSolidSphere(1, 20, 20);
	}

	//Shoulder
	void TransformShoulder(float **params) {
		glRotated(*params[0], 0, 1, 0);
		glTranslated(0.5, 1.8, 0);
		glRotated(*params[1], 0, 1, 0);
	}
	void DrawShoulder(float **params) {
		glutSolidSphere(0.25, 15, 15);
		glTranslated(0.5, 0, 0);
		glScaled(1, 0.2, 0.2);
		glutSolidCube(1);
	}

	//Elbow
	void TransformElbow(float **params) { glTranslated(1, 0, 0); glRotated(*params[0], 1, 0, 0); }
	void DrawElbow(float **params) {
		glutSolidSphere(0.15, 15, 15);
		glTranslated(0, 0.4, 0);
		glScaled(0.2, 0.8, 0.2);
		glutSolidCube(1);
	}

	//Wrist
	void TransformWrist(float **params) { glTranslated(0, 0.8, 0); }
	void DrawWrist(float **params) {
		glutSolidSphere(0.1, 15, 15);
		glPushMatrix();
		glRotated(*params[0] / -2, 1, 0, 0);
		glScaled(0.1, 0.5, 0.1);
		glTranslated(0, 0.25, 0);
		glutSolidCube(1);
		glPopMatrix();

		glPushMatrix();
		glRotated(*params[0] / 2, 1, 0, 0);
		glScaled(0.1, 0.5, 0.1);
		glTranslated(0, 0.25, 0);
		glutSolidCube(1);
		glPopMatrix();
	}

	//Main
	Node Torzo, Head, ShoulderR, ElbowR, WristR, ShoulderL, ElbowL, WristL;
	float aTorso, aHead, aShoulderR, aElbowR, aWristR, aShoulderL, aElbowL, aWristL;
	void Robot() {
		Torzo.Child = new Node*[3]{ &Head,&ShoulderR,&ShoulderL }; Torzo.ChildCount = 3;
		ShoulderR.Child = new Node*[1]{ &ElbowR }; ShoulderR.ChildCount = 1;
		ElbowR.Child = new Node*[1]{ &WristR }; ElbowR.ChildCount = 1;
		ShoulderL.Child = new Node*[1]{ &ElbowL }; ShoulderL.ChildCount = 1;
		ElbowL.Child = new Node*[1]{ &WristL }; ElbowL.ChildCount = 1;

		Torzo.Transform = TrasnformTorzo;
		Torzo.paramsT = new float*[1]{ &aTorso };
		Torzo.Draw = DrawTorzo;

		Head.Transform = TransformHead;
		Head.paramsT = new float*[1]{ &aHead };
		Head.Draw = DrawHead;

		ShoulderR.Transform = TransformShoulder;
		ShoulderR.paramsT = new float*[2]{ new float(180), &aShoulderR };
		ShoulderR.Draw = DrawShoulder;

		ElbowR.Transform = TransformElbow;
		ElbowR.paramsT = new float*[1]{ &aElbowR };
		ElbowR.Draw = DrawElbow;

		WristR.Transform = TransformWrist;
		WristR.paramsD = new float*[1]{ &aWristR };
		WristR.Draw = DrawWrist;

		ShoulderL.Transform = TransformShoulder;
		ShoulderL.paramsT = new float*[2]{ new float(0), &aShoulderL };
		ShoulderL.Draw = DrawShoulder;

		ElbowL.Transform = TransformElbow;
		ElbowL.paramsT = new float*[1]{ &aElbowL };
		ElbowL.Draw = DrawElbow;

		WristL.Transform = TransformWrist;
		WristL.paramsD = new float*[1]{ &aWristL };
		WristL.Draw = DrawWrist;
	}
};

struct Robot {
public:
	float aTorso, aHead, aShoulderR, aElbowR, aWristR, aShoulderL, aElbowL, aWristL;
	float *position; float *colorDiffuse; float *colorAmbient;

	int actBaile, posCount; float trans;
	float ***bailes; int *cantPos;

	Robot() {
		position = new float[3]{ 0,0,0 };
		colorDiffuse = new float[4]{ 0.1f, 0.1f, 0.1f, 1.0f };
		colorAmbient = new float[4]{ 0.3f, 0.3f, 0.3f, 1.0f };
		RobotFunc::Robot();
		//Bailes
		cantPos = new int[1]{ 10 };
		bailes = new float**[1];
		bailes[0] = new float*[10];
		bailes[0][0] = new float[8]{ 5, 0, 0, 0, 0, 0, 0, 0 };
		bailes[0][1] = new float[8]{ -5, 0, 0, 0, 0, -95, 0, 0 };
		bailes[0][2] = new float[8]{ 5, 0, 90, 0, 0, -95, 0, 0 };
		bailes[0][3] = new float[8]{ -5, 0, 90, 0, 0, -90, 95, 0 };
		bailes[0][4] = new float[8]{ 5, 0, 90, -80, 0, -90, 95, 0 };
		bailes[0][5] = new float[8]{ -5, 0, 90, -80, 0, -10, 95, 0 };
		bailes[0][6] = new float[8]{ 5, 0, 0, -80, 0, -10, 95, 0 };
		bailes[0][7] = new float[8]{ -5, 0, 0, -80, 65, -10, 95, -80 };
		bailes[0][8] = new float[8]{ 5, 0, 0, -80, 65, -10, 0, -80 };
		bailes[0][9] = new float[8]{ -5, 0, 0, 5, 65, -10, 0, -80 };
	}

	void DrawRobot() {
		glPushMatrix();
		glTranslated(position[0], position[1], position[2]);
		glMaterialfv(GL_FRONT, GL_AMBIENT, colorAmbient); //RGBA de la luz ambiental
		glMaterialfv(GL_FRONT, GL_DIFFUSE, colorDiffuse); //RGBA de diffuse, el material
		if (actBaile != 0) {
			actPocisionBaile(bailes[actBaile - 1], posCount, cantPos[actBaile - 1], trans);
			actParamsFunc();
			Node::DrawObj(&RobotFunc::Torzo);
			trans += 0.05;
			if (trans > 1) { trans = 0; posCount++; }
			if (posCount >= cantPos[actBaile - 1]) { posCount = 0; }
		}
		else {
			actParamsFunc();
			Node::DrawObj(&RobotFunc::Torzo);
		}

		glPopMatrix();
	}

	void actPocisionBaile(float **posicion, int sigPos, int totPos, float trans) { //trans va de 0 a 1
		aTorso = posicion[(sigPos - 1 + totPos) % totPos][0] + trans*(posicion[sigPos][0] - posicion[(sigPos - 1 + totPos) % totPos][0]);
		aHead = posicion[(sigPos - 1 + totPos) % totPos][1] + trans*(posicion[sigPos][1] - posicion[(sigPos - 1 + totPos) % totPos][1]);
		aShoulderR = posicion[(sigPos - 1 + totPos) % totPos][2] + trans*(posicion[sigPos][2] - posicion[(sigPos - 1 + totPos) % totPos][2]);
		aElbowR = posicion[(sigPos - 1 + totPos) % totPos][3] + trans*(posicion[sigPos][3] - posicion[(sigPos - 1 + totPos) % totPos][3]);
		aWristR = posicion[(sigPos - 1 + totPos) % totPos][4] + trans*(posicion[sigPos][4] - posicion[(sigPos - 1 + totPos) % totPos][4]);
		aShoulderL = posicion[(sigPos - 1 + totPos) % totPos][5] + trans*(posicion[sigPos][5] - posicion[(sigPos - 1 + totPos) % totPos][5]);
		aElbowL = posicion[(sigPos - 1 + totPos) % totPos][6] + trans*(posicion[sigPos][6] - posicion[(sigPos - 1 + totPos) % totPos][6]);
		aWristL = posicion[(sigPos - 1 + totPos) % totPos][7] + trans*(posicion[sigPos][7] - posicion[(sigPos - 1 + totPos) % totPos][7]);
	}
	void actParamsFunc() {
		RobotFunc::aTorso = aTorso;
		RobotFunc::aHead = aHead;
		RobotFunc::aShoulderR = aShoulderR;
		RobotFunc::aElbowR = aElbowR;
		RobotFunc::aWristR = aWristR;
		RobotFunc::aShoulderL = aShoulderL;
		RobotFunc::aElbowL = aElbowL;
		RobotFunc::aWristL = aWristL;
	}

	void ImprimirPocision() {
		cout << "{ " << aTorso << ", " << aHead << ", " << aShoulderR << ", " << aElbowR << ", " << aWristR << ", " << aShoulderL << ", " << aElbowL << ", " << aWristL << "}" << endl;
	}
};


Robot robot1, robot2, robot3, robot4, robot5;
void init() {
	robot1.position = new float[3]{ 0,0, 2 };
	robot2.position = new float[3]{ -6,0,-1 };
	robot3.position = new float[3]{ -2,0,-1 };
	robot4.position = new float[3]{ 2,0,-1 };
	robot5.position = new float[3]{ 6,0,-1 };

	robot1.colorAmbient = new float[4]{ 0.05f, 0.2f, 0.4f, 1.0f }; robot1.colorDiffuse = new float[4]{ 0.1f, 0.5f, 0.8f, 1.0f };
	robot2.colorAmbient = new float[4]{ 0.4f, 0.2f, 0.05f, 1.0f }; robot2.colorDiffuse = new float[4]{ 0.8f, 0.5f, 0.1f, 1.0f };
	robot3.colorAmbient = new float[4]{ 0.2f, 0.4f, 0.05f, 1.0f }; robot3.colorDiffuse = new float[4]{ 0.5f, 0.8f, 0.1f, 1.0f };
	robot4.colorAmbient = new float[4]{ 0.4f, 0.1f, 0.05f, 1.0f }; robot4.colorDiffuse = new float[4]{ 0.8f, 0.1f, 0.1f, 1.0f };
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//DrawReferenceFrame();
	robot1.DrawRobot();
	robot2.DrawRobot();
	robot3.DrawRobot();
	robot4.DrawRobot();
	robot5.DrawRobot();

	glutSwapBuffers(); //3D
}

void keyboard(unsigned char key, int x, int y) { //DUDA: X, Y??
	switch (key) {
	case 27: exit(0); break;
	case 'P': case'p': robot2.actBaile = 1; break;
	case 'O': case'o': robot3.actBaile = 1; break;
	case 'I': case'i': robot4.actBaile = 1; break;
	case 'U': case'u': robot5.actBaile = 1; break;
	case 'Q': robot1.aTorso += 5; break;
	case 'q': robot1.aTorso -= 5; break;
	case 'W': robot1.aHead += 5; break;
	case 'w': robot1.aHead -= 5; break;
	case 'A': robot1.aShoulderL -= 5; break;
	case 'a': robot1.aShoulderL += 5; break;
	case 'S': robot1.aShoulderR += 5; break;
	case 's': robot1.aShoulderR -= 5; break;
	case 'Z': robot1.aElbowL += 5; break;
	case 'z': robot1.aElbowL -= 5; break;
	case 'X': robot1.aElbowR -= 5; break;
	case 'x': robot1.aElbowR += 5; break;
	case 'E': robot1.aWristL += 5; break;
	case 'e': robot1.aWristL -= 5; break;
	case 'R': robot1.aWristR -= 5; break;
	case 'r': robot1.aWristR += 5; break;
	case 'D': case 'd': robot1.ImprimirPocision(); break;
	}
	glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: break;
	case GLUT_KEY_DOWN:  break;
	case GLUT_KEY_LEFT:  break;
	case GLUT_KEY_RIGHT:  break;
	}
	glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		switch (btn) {
		case GLUT_LEFT_BUTTON: break;
		case GLUT_RIGHT_BUTTON:  break;
		}
	}
	glutPostRedisplay(); //Redibujar
}

void mouse_routine(int x, int y) {

}

//OpenGL

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //set the viewport size, based on fuction input 
	glMatrixMode(GL_PROJECTION); glLoadIdentity(); //set the projection matrix set as identity	

												   //2D
												   //gluOrtho2D(0.0, (GLsizei)w, (GLsizei)h, 0.0);

												   //3D
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.1, 70.0); //then set perspective projection parameters based on aspect ratio
	glMatrixMode(GL_MODELVIEW); glLoadIdentity(); //set the model view matrix to identity
	gluLookAt(4.0, 5.0, 10.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void OpenGL_Initialize(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	// 3D
	glShadeModel(GL_SMOOTH); //Set shading model to use
	glEnable(GL_DEPTH_TEST); //for hidden surface removal
							 //glShadeModel(GL_FLAT);

							 //Material
							 //glMaterialfv(GL_FRONT, GL_AMBIENT, new float[4]{ 0.05f, 0.2f, 0.4f, 1.0f }); //RGBA de la luz ambiental
							 //glMaterialfv(GL_FRONT, GL_DIFFUSE, new float[4]{ 0.1f, 0.5f, 0.8f, 1.0f }); //RGBA de diffuse, el material
	glMaterialfv(GL_FRONT, GL_EMISSION, new float[4]{ 0.0f, 0.0f, 0.0f, 1.0f }); //RGBA de emision del material
	glMaterialfv(GL_FRONT, GL_SPECULAR, new float[4]{ 0.0f, 0.0f, 0.0f, 1.0f }); //RGBA de specular el shinny
	glMaterialf(GL_FRONT, GL_SHININESS, 0.0); //specular exponent [0, 128] - high:100.0f  low:5.0  no:0.0

											  //Ligths
	glLightfv(GL_LIGHT0, GL_AMBIENT, new float[4]{ 1.0f, 1.0f, 1.0f, 1.0f }); //Color ambiente
	glLightfv(GL_LIGHT0, GL_SPECULAR, new float[4]{ 0.0f, 0.0f, 0.0f, 1.0f }); //Color shine	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, new float[4]{ 1.0f, 1.0f, 1.0f, 1.0f }); //Color object
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, new float[4]{ 0.4f, 0.4f, 0.4f, 1.0f }); //RGBA intensity of ambient
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0); //How specular is calculated  
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0); //One Side or Two Side

	glLightfv(GL_LIGHT0, GL_POSITION, new float[4]{ 5.0f, 3.0f, 5.0f, 1.0f });

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

int main(int argc, char** argv) {
	srand(time(NULL));

	//GLUT INITIALIZATION
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL");

	//Additional initialization
	OpenGL_Initialize();
	init();

	//register callback fuctions
	glutDisplayFunc(display);
	glutPassiveMotionFunc(mouse_routine);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);
	glutMouseFunc(mouse);
	glutSpecialFunc(processSpecialKeys);

	//Do main loop
	glutMainLoop();

	//Exit
	return 0;
}

void drawAxis(double length) { // Draw a z-axis with a cone at the end - From CG using OpenGL - Hill and Kelley
	glPushMatrix();
	glBegin(GL_LINES); // Draw line
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, length);
	glEnd();
	glTranslated(0.0, 0.0, length - 0.2); // Draw cone
	glutSolidCone(0.04, 0.2, 12, 9);
	glPopMatrix();
}

void DrawReferenceFrame() {
	// Draw reference frame
	glLineWidth(2.0);
	// z axis
	glColor3f(0.0, 0.0, 1.0);
	drawAxis(1.0);
	// y axis
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	glRotated(-90.0, 1.0, .0, 0.0); drawAxis(1.0);
	glPopMatrix();
	// x axis
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glRotated(90.0, 0.0, 1.0, 0.0); drawAxis(1.0);
	glPopMatrix();
}