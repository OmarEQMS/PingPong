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
double Physics::dt = 0.001; //10ms

//Objetos en escena
Light luz1(GL_LIGHT1, true);

//Objetos
Jugador jugador1, jugador2;
Marcador marcador;
MesaPing mesa;
Estadio estadio;
Pelota pelota;

void Initialization() {
	//Ligths
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, new float[4]{ 0.8f, 0.8f, 0.8f, 1.0f }); //RGB for Global Illumination
	luz1.Position(0, 1, 0); luz1.PointOrVector(0); luz1.Assign(0.4, 0.8, 0.8);

	//Init
	marcador.Init();
	jugador1.Init(1);
	jugador2.Init(-1);
	mesa.Init();
	estadio.Init();
	pelota.Init();

	BoundingBox::AddCollisionDetection(pelota.box.index, mesa.box[0].index, true);
	BoundingBox::AddCollisionDetection(pelota.box.index, jugador1.box.index, true);
	BoundingBox::AddCollisionDetection(pelota.box.index, jugador2.box.index, true);
	BoundingBox::AddCollisionDetection(pelota.box.index, estadio.box[0].index, true);
	BoundingBox::AddCollisionDetection(pelota.box.index, estadio.box[1].index, true);
	BoundingBox::AddCollisionDetection(pelota.box.index, estadio.box[2].index, true);
	BoundingBox::AddCollisionDetection(pelota.box.index, estadio.box[3].index, true);
	BoundingBox::AddCollisionDetection(pelota.box.index, estadio.box[4].index, true);
	BoundingBox::AddCollisionDetection(pelota.box.index, estadio.box[5].index, true);
}

void Reshape(int w, int h) {}
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	jugador1.Reshape();
	GameObject::DrawAll();
	jugador2.Reshape();
	GameObject::DrawAll();
	glutSwapBuffers();
}
void Update() {
	marcador.Update();
	jugador1.Update();
	jugador2.Update();
	mesa.Update();
	estadio.Update();
	pelota.Update();
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
	jugador1.KesysRot((key == 'Z' || key == 'z'), (key == 'X' || key == 'x'), (key == 'Q' || key == 'q'), (key == 'E' || key == 'e'), (key == 'R' || key == 'r'), (key == 'F' || key == 'f'));
	jugador2.KesysRot((key == '4'), (key == '5'), (key == '1'), (key == '2'), (key == '7'), (key == '8'));
	//Cameras
	jugador1.Camera.keyboard(key, x, y);
	jugador2.Camera.keyboard(key, x, y);
	//Marcados
	switch (key) {
	case 27: exit(0); break;
	case 'U': case 'u': marcador.IncJ1(); break;
	case 'I': case 'i': marcador.DecJ1(); break;
	case 'O': case 'o': marcador.IncJ2(); break;
	case 'P': case 'p': marcador.DecJ2(); break;
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