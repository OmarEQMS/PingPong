/*
#include "Libraries.h"
#include "LibrariesRaycasting.h"
Vertex2 Vertex2::zero;
Vertex3 Vertex3::zero;
vector<Light*> Light::luces;
vector<GameObject*> GameObject::GameObjects;
vector<BoundingBox*> BoundingBox::BoundingBoxes;
vector<vector<int>> BoundingBox::collisionDetection;


//Objetos en escena
Light luz1;
TransformMatrix matrix;
CameraTool CameraMovement(48);
CameraTool VirtualCameraMove(60, 0.1,100);
VirtualCamera virtualCam(480, 480, 0.1, 60, Vertex3(0,0,3), Vertex3(0,0,0));

//Objetos
ParametricSphere esfera(Vertex3(0, 0, 0), 1);

void Initialization() {
	//Ligths
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, new float[4]{ 0.8f, 0.8f, 0.8f, 1.0f }); //RGB for Global Illumination
	luz1.Position(2, 2, 2); luz1.Assign(0.4, 0.6, 0.8);
	//Camera
	CameraMovement.CamaraEye.SetVertices(24, 24, 0);

	//Objetos
	virtualCam.lights.push_back(&luz1);
	virtualCam.objects.push_back(&esfera);
	esfera.material.Assign(NULL, new float[3]{ 0.05f,0.2f,0.05f }, new float[3]{ 0.1f,0.8f,0.1f }, new float[3]{ 0.2f,0.8f,0.2f }, new float(16));
}

void display(void) {
	//Camara
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CameraMovement.Set(); //Necesario para mover la camara
	//Default 
	ReferenceFrame::DrawReferenceFrame();
	//Objects
	virtualCam.CamaraEye = VirtualCameraMove.CamaraEye;
	virtualCam.CamaraCenter = VirtualCameraMove.CamaraCenter;
	virtualCam.Display();
	glutSwapBuffers(); //3D
}

//KEYBOARD MOUSE
void keyboard(unsigned char key, int x, int y) { //DUDA: X, Y??
	VirtualCameraMove.keyboard(key, x, y);
	switch (key) {
		case 27: exit(0); break;
	}
}
void processSpecialKeys(int key, int x, int y) {
	VirtualCameraMove.processSpecialKeys(key, x, y);
	switch (key) {
	case GLUT_KEY_UP: break;
	case GLUT_KEY_DOWN:  break;
	case GLUT_KEY_LEFT:  break;
	case GLUT_KEY_RIGHT:  break;
	}
}
void mouse(int btn, int state, int x, int y) {
	VirtualCameraMove.mouse(btn, state, x, y);
}
void mouse_routine(int x, int y) {
	VirtualCameraMove.mouse_idle(x, y);
}

//OEPN GL
int main(int argc, char** argv) {	
	srand(time(NULL));

	//GLUT INITIALIZATION	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(480, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL");

	// Setting
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH); //Set shading model to use
	glEnable(GL_DEPTH_TEST); //for hidden surface removal	
	glPolygonMode(GL_FRONT, GL_FILL); //Only Front Face
	glCullFace(GL_BACK); glEnable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	//Callback Fuctions
	glutPassiveMotionFunc(mouse_routine);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutSpecialFunc(processSpecialKeys);
	glutDisplayFunc(display);
	glutIdleFunc(display);

	//Initialization	
	Initialization();

	//Do main loop
	glutMainLoop();

	//Exit
	return 0;
}
*/