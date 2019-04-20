#include "Libraries.h"
Vertex2 Vertex2::zero;
Vertex3 Vertex3::zero;
vector<Light*> Light::luces;

//Objetos en escena
Light luz1(GL_LIGHT1, true);
Light luz2(GL_LIGHT2, true);
TransformMatrix matrix;
CameraTool CameraMovement(60, 0.1, 70);
GameObject centerCam(&CameraMovement.CamaraCenter, new Vertex3());

//Objetos
GameObject raqueta, mesaPing, stadium, box1, box2, box3;

void init() {
	//Ligths
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, new float[4]{ 0.8f, 0.8f, 0.8f, 1.0f }); //RGB for Global Illumination
	luz1.Position(0, 1, 0); luz1.PointOrVector(0); luz1.Assign(0.4, 0.8, 0.8);
	luz2.Position(0, 3, 0); luz2.SetCutoff(new float[3]{ 0,-1,0 }, 20, 0); luz2.Assign(0.4, 0.8, 0.8);
	//centerCam	
	BasicShapesTool::MakeShape(&centerCam, 0, Shape::Sphere, 0.1, 0.1, 0.1, 10);

	//stadium
	matrix.Identity(); matrix.Scale(0.03, 0.03, 0.03); matrix.Translated(0, -5.381100, 0);
	stadium.position->SetVertices(0, 0, 0);
	ReadModelOBJ::readOBJ(&stadium, -1, "estadio.obj"); //Las meshes se crean solitas por lo mtllibs
	stadium.MultVertices(matrix);

	//mesaPing
	matrix.Identity(); matrix.Translated(0, -0.1, 0); matrix.RotateY(90); matrix.Scale(0.08, 0.08, 0.08);
	mesaPing.position->SetVertices(0, 0, 0);
	ReadModelOBJ::readOBJ(&mesaPing, -1, "PingPongTable.obj"); //Las meshes se crean solitas por lo mtllibs
	mesaPing.MultVertices(matrix);

	//raqueta
	matrix.Identity(); matrix.Translated(0, 0.35, 1.2); matrix.RotateY(90); matrix.RotateZ(-90); matrix.Scale(0.1, 0.1, 0.1);
	raqueta.position->SetVertices(0, 0, 0);
	ReadModelOBJ::readOBJ(&raqueta, -1, "Raqueta.obj"); //Las meshes se crean solitas por lo mtllibs
	raqueta.MultVertices(matrix);
	raqueta.CalculateBoundingBox();

	//boxes
	box1.position->SetVertices(0, 0, 0);
	BasicShapesTool::MakeShape(&box1, 0, Shape::Cube, 1.22, 0.508, 2.2, 0);
	matrix.Identity(); matrix.Translated(0, 0.254, 0);
	box1.MultVerticesMesh(matrix, 0);

	box2.position->SetVertices(0, 0, 0);	
	BasicShapesTool::MakeShape(&box2, 0, Shape::Cube, 1.22, 0.08, 0.01, 0);
	matrix.Identity(); matrix.Translated(0, 0.548, 0);
	box2.MultVerticesMesh(matrix, 0);

	box3.position->SetVertices(0, 0, 0);
	BasicShapesTool::MakeShape(&box3, 0, Shape::Cube, 2, 2, 4, 0);
	matrix.Identity(); matrix.Translated(0, 1, 0);
	box3.MultVerticesMesh(matrix, 0);

}

void display(void) {
	//Camara
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CameraMovement.Set(); //Necesario para mover la camara

	//Default 
	ReferenceFrame::DrawReferenceFrame();
	centerCam.Draw();

	//Objetos
	raqueta.Draw();
	mesaPing.Draw();
	stadium.Draw();
	//box1.Draw();
	//box2.Draw();
	//box3.Draw();

	glutSwapBuffers(); //3D
}

//KEYBOARD MOUSE
void keyboard(unsigned char key, int x, int y) { //DUDA: X, Y??
	CameraMovement.keyboard(key, x, y);
	switch (key) {
		case 27: exit(0); break;
	}
}
void processSpecialKeys(int key, int x, int y) {
	CameraMovement.processSpecialKeys(key, x, y);
	switch (key) {
	case GLUT_KEY_UP: break;
	case GLUT_KEY_DOWN:  break;
	case GLUT_KEY_LEFT:  break;
	case GLUT_KEY_RIGHT:  break;
	}
}
void mouse(int btn, int state, int x, int y) {
	CameraMovement.mouse(btn, state, x, y);
}
void mouse_routine(int x, int y) {
	CameraMovement.mouse_idle(x, y);
}

//OEPN GL
int main(int argc, char** argv) {	
	srand(time(NULL));

	//GLUT INITIALIZATION
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	// 3D Setting
	glShadeModel(GL_SMOOTH); //Set shading model to use
	glEnable(GL_DEPTH_TEST); //for hidden surface removal
	//Only Front Face
	glPolygonMode(GL_FRONT, GL_FILL);
	glCullFace(GL_BACK); glEnable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	//Additional initialization
	init();

	//register callback fuctions
	glutDisplayFunc(display);
	glutPassiveMotionFunc(mouse_routine);
	//glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);
	glutMouseFunc(mouse);
	glutSpecialFunc(processSpecialKeys);

	//Do main loop
	glutMainLoop();

	//Exit
	return 0;
}