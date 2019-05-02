///*
#include "Libraries.h"
#include "Libraries.h"
#include "PingPong.h"
#include "LibrariesRaycasting.h"

Vertex2 Vertex2::zero;
Vertex3 Vertex3::zero;
vector<Light*> Light::luces;
vector<GameObject*> GameObject::GameObjects;
vector<BoundingBox*> BoundingBox::BoundingBoxes;
vector<vector<int>> BoundingBox::collisionDetection;
vector<Physics*> Physics::Physicses;
Vertex3 Physics::globalAcceleration(0, -9.81, 0);
double Physics::dt = 0.1;

//Objetos en escena
Light luz1(GL_LIGHT1, true);
TransformMatrix matrix;

//Objetos
GameObject mesaPing, stadium, pelota, box;
Jugador jugador1, jugador2;
Marcador marcador;

void Initialization() {
	//Ligths
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, new float[4]{ 0.8f, 0.8f, 0.8f, 1.0f }); //RGB for Global Illumination
	luz1.Position(0, 1, 0); luz1.PointOrVector(0); luz1.Assign(0.4, 0.8, 0.8);

	//Jugadores
	marcador.Init();
	jugador1.Init(1);
	jugador2.Init(-1);

	//stadium
	stadium.Init("estadio");
	matrix.Identity(); matrix.Scale(0.03, 0.03, 0.03); matrix.Translated(0, -5.381100, 0);
	stadium.position->SetVertices(0, 0, 0);
	ReadModelOBJ::readOBJ(&stadium, -1, "estadio.obj"); //Las meshes se crean solitas por lo mtllibs
	matrix.MultGameObject(&stadium);

	//mesaPing
	mesaPing.Init("mesa");
	matrix.Identity(); matrix.Translated(0, -0.1, 0); matrix.RotateY(90); matrix.Scale(0.08, 0.08, 0.08);
	mesaPing.position->SetVertices(0, 0, 0);
	ReadModelOBJ::readOBJ(&mesaPing, -1, "PingPongTable.obj"); //Las meshes se crean solitas por lo mtllibs
	matrix.MultGameObject(&mesaPing);
	
	//pelotita	
	pelota.Init("pelota");
	pelota.position->SetVertices(0, 0.75, 0);
	int m; pelota.pushMaterial(m);
	pelota.pushMesh(0, -1);
	BasicShapesTool::MakeShape(&pelota, 0, Shape::Sphere, 0.05, 0.05, 0.05, 10);
	pelota.material[m].Ambient(0.8, 0.1, 0.1);
	pelota.material[m].Diffuse(1, 0.2, 0.2);
	pelota.material[m].Specular(1, 0.8, 0.8);
	pelota.material[m].Shinnes(128);

	//Boxes
	box.Init("box", false);
	box.position->SetVertices(0, 0, 0);	
	BasicShapesTool::MakeShape(&box, 0, Shape::Cube, 1.22, 0.508, 2.2, 0); //Mesa
	matrix.Identity(); matrix.Translated(0, 0.254, 0);
	matrix.MultGameObjectMesh(&box, 0);	
	BasicShapesTool::MakeShape(&box, 1, Shape::Cube, 1.22, 0.08, 0.01, 0); //Red
	matrix.Identity(); matrix.Translated(0, 0.548, 0);
	matrix.MultGameObjectMesh(&box, 1);	
	BasicShapesTool::MakeShape(&box, 2, Shape::Cube, 3, 0.5, 5, 0); //Environment
	matrix.Identity(); matrix.Translated(0, -0.25, 0);
	matrix.MultGameObjectMesh(&box, 2);
	BasicShapesTool::MakeShape(&box, 3, Shape::Cube, 3, 0.5, 5, 0);
	matrix.Identity(); matrix.Translated(0, 2.25, 0);
	matrix.MultGameObjectMesh(&box, 3);
	BasicShapesTool::MakeShape(&box, 4, Shape::Cube, 2, 2, 0.5, 0);
	matrix.Identity(); matrix.Translated(0, 1, 2.25);
	matrix.MultGameObjectMesh(&box, 4);
	BasicShapesTool::MakeShape(&box, 5, Shape::Cube, 2, 2, 0.5, 0);
	matrix.Identity(); matrix.Translated(0, 1, -2.25);
	matrix.MultGameObjectMesh(&box, 5);
	BasicShapesTool::MakeShape(&box, 6, Shape::Cube, 0.5, 2, 5, 0);
	matrix.Identity(); matrix.Translated(1.25, 1, 0);
	matrix.MultGameObjectMesh(&box, 6);
	BasicShapesTool::MakeShape(&box, 7, Shape::Cube, 0.5, 2, 5, 0);
	matrix.Identity(); matrix.Translated(-1.25, 1, 0);
	matrix.MultGameObjectMesh(&box, 7);

}

void Reshape(int w, int h) {}
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	jugador1.Camera.Set(0, 0, 2, 1); //Camara
	GameObject::DrawAll();
	jugador2.Camera.Set(1, 0, 2, 1); //Camara
	GameObject::DrawAll();
	glutSwapBuffers();
}
void Update() {
	marcador.Update();
}
void collisionsANDphysics() {
	BoundingBox::CheckCollisions();
	Physics::UpdatePhysics();
}
void Idle(void) {
	thread update(Update);
	thread dibujar(glutPostRedisplay);
	update.join();
	dibujar.join();
}

//KEYBOARD MOUSE
void keyboard(unsigned char key, int x, int y) { 
	//Keys Jugadores
	jugador1.Kesys((key == 'W' || key == 'w'), (key == 'S' || key == 's'), (key == 'A' || key == 'a'), (key == 'D' || key == 'd'));
	jugador1.KesysRot((key == 'Z' || key == 'z'), (key == 'X' || key == 'x'), (key == 'Q' || key == 'q'), (key == 'E' || key == 'e'));
	jugador2.KesysRot((key == '4'), (key == '5'), (key == '1'), (key == '2'));
	//Cameras
	jugador1.Camera.keyboard(key, x, y);
	jugador2.Camera.keyboard(key, x, y);
	//Marcados
	switch (key) {
	case 27: exit(0); break;
	case 'U': case 'u': marcador.jugador1++; break;
	case 'I': case 'i': marcador.jugador1--; break;
	case 'O': case 'o': marcador.jugador2++; break;
	case 'P': case 'p': marcador.jugador2--; break;
	//Cameras
	case 'M': case 'm': 
		if (jugador1.Camera.ActivePanTilt) {
			jugador1.Camera.ActivePanTilt = false;
			jugador2.Camera.ActivePanTilt = true;
		} else {
			jugador1.Camera.ActivePanTilt = true;
			jugador2.Camera.ActivePanTilt = false;
		}
	break;
	}	
}
void processSpecialKeys(int key, int x, int y) {
	//Jugador
	jugador2.Kesys((key == GLUT_KEY_UP), (key == GLUT_KEY_DOWN), (key == GLUT_KEY_LEFT), (key == GLUT_KEY_RIGHT));
	//Cameras
	jugador1.Camera.processSpecialKeys(key, x, y);
	jugador2.Camera.processSpecialKeys(key, x, y);
}
void mouse(int btn, int state, int x, int y) {
	jugador1.Camera.mouse(btn, state, x, y);
	jugador2.Camera.mouse(btn, state, x, y);
}
void mouse_routine(int x, int y) {
	jugador1.Camera.mouse_idle(x, y);
	jugador2.Camera.mouse_idle(x, y);
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
	glutMouseFunc(mouse);
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);

	//Initialization	
	Initialization();

	//Timer
	Timer t = Timer();
	t.setInterval(collisionsANDphysics, Physics::dt*1000*10);

	//Do main loop
	glutMainLoop();

	//Exit
	return 0;
}

//*/