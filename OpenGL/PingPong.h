#pragma once
//Debe tener Init y Update

struct Marcador {
	GameObject banner, marcadorA, marcadorB;
	Texture numeros[11];
	int jugador1, jugador2;

	Marcador() {}
	void Init() {
		//BMPs
		ReadBitMap::ReadBMPFile(&numeros[0], "0.bmp");
		ReadBitMap::ReadBMPFile(&numeros[1], "1.bmp");
		ReadBitMap::ReadBMPFile(&numeros[2], "2.bmp");
		ReadBitMap::ReadBMPFile(&numeros[3], "3.bmp");
		ReadBitMap::ReadBMPFile(&numeros[4], "4.bmp");
		ReadBitMap::ReadBMPFile(&numeros[5], "5.bmp");
		ReadBitMap::ReadBMPFile(&numeros[6], "6.bmp");
		ReadBitMap::ReadBMPFile(&numeros[7], "7.bmp");
		ReadBitMap::ReadBMPFile(&numeros[8], "8.bmp");
		ReadBitMap::ReadBMPFile(&numeros[9], "9.bmp");
		ReadBitMap::ReadBMPFile(&numeros[10], "guion.bmp");
		//Matrix
		TransformMatrix matrix;
		//Init - Pos and Rot
		banner.Init(new Vertex3(2.65, 1.7, 0), new Vertex3(0, 0, 0), true);
		marcadorA.Init(new Vertex3(0, 1.7, 4.7), new Vertex3(0, -90, 0), true);
		marcadorB.Init(new Vertex3(0, 1.7, -4.7), new Vertex3(0, 90, 0), true);
		//Texture
		int index;
		//Banner
		banner.pushTexture(index);
		banner.pushMesh(-1, index);
		ReadBitMap::ReadBMPFile(&banner.textura[index], "pingpong.bmp");
		//Marcador
		for (int i = 0; i < 2; i++) {
			marcadorA.pushTexture(index);
			marcadorA.pushMesh(-1, index);
			marcadorB.pushTexture(index);
			marcadorB.pushMesh(-1, index);
		}
		//Meshes
		BasicShapesTool::MakeShape(&banner, 0, Shape::Quad, 0.5, 0.5, 2, 1);
		matrix.Identity(); matrix.RotateZ(-90);
		matrix.MultGameObjectMesh(&banner, 0);
		//MeshesA
		BasicShapesTool::MakeShape(&marcadorA, 0, Shape::Quad, 0.5, 0.5, 0.5, 1);
		BasicShapesTool::MakeShape(&marcadorA, 1, Shape::Quad, 0.5, 0.5, 0.5, 1);
		matrix.Identity(); matrix.Translated(0, 0, -0.25); matrix.RotateZ(-90);
		matrix.MultGameObjectMesh(&marcadorA, 0);
		matrix.Identity(); matrix.Translated(0, 0, 0.25); matrix.RotateZ(-90);
		matrix.MultGameObjectMesh(&marcadorA, 1);
		//MeshesB
		BasicShapesTool::MakeShape(&marcadorB, 0, Shape::Quad, 0.5, 0.5, 0.5, 1);
		BasicShapesTool::MakeShape(&marcadorB, 1, Shape::Quad, 0.5, 0.5, 0.5, 1);
		matrix.Identity(); matrix.Translated(0, 0, -0.25); matrix.RotateZ(-90);
		matrix.MultGameObjectMesh(&marcadorB, 0);
		matrix.Identity(); matrix.Translated(0, 0, +0.25); matrix.RotateZ(-90);
		matrix.MultGameObjectMesh(&marcadorB, 1);
		//Set 0-0
		jugador1 = 0; jugador2 = 0;
		Update();
	}

	void IncJ1() { jugador1++; }
	void IncJ2() { jugador2++; }
	void DecJ1() { jugador1--; }
	void DecJ2() { jugador2--; }

	void Update() {
		//A
		marcadorA.textura[marcadorA.meshes[0].textura] = numeros[(jugador1 / 10)];
		marcadorA.textura[marcadorA.meshes[1].textura] = numeros[(jugador1 % 10)];
		//B
		marcadorB.textura[marcadorB.meshes[0].textura] = numeros[(jugador2 / 10)];
		marcadorB.textura[marcadorB.meshes[1].textura] = numeros[(jugador2 % 10)];
	}

};

struct Pelota {
public:
	static vector<Pelota*> pelotas;
public:
	int index;
	Marcador* marcador;
	Vertex3* position;
	Vertex3* rotation;
	GameObject pelota;
	BoundingBox box;
	Physics phy;
	int estado;

	Pelota() {
		index = pelotas.size();
		pelotas.push_back(this);
	}

	void Init(Marcador* marcador) {
		//Marcador Reference
		this->marcador = marcador;
		//Pos and Rot
		position = new Vertex3(0, 0.8, 0);
		rotation = new Vertex3(0, 0, 0);
		//pelotita	
		pelota.Init(position, rotation, true);
		int m; pelota.pushMaterial(m);
		pelota.pushMesh(0, -1);
		BasicShapesTool::MakeShape(&pelota, 0, Shape::Sphere, 0.05, 0.05, 0.05, 10);
		pelota.material[m].Ambient(0.8, 0.1, 0.1);
		pelota.material[m].Diffuse(1, 0.2, 0.2);
		pelota.material[m].Specular(1, 0.8, 0.8);
		pelota.material[m].Shinnes(128);
		//Physics
		phy.Init(position, 1);
		phy.AddImpulse(Vertex3(0, 0, -2));
		estado = 2;
		//Bounding Box
		box.Init("ball", position, rotation, &pelota, &phy, index, OnCollision);
	}

	static void OnCollision(int id, BoundingBox* other, Vertex3 direction) {
		if (other->colliderName == "J1") {
			pelotas[id]->estado = 2; //La toca el jugador 1
		}else if (other->colliderName == "J2") {
			pelotas[id]->estado = -2; //La toca el jugador -1
		}else if (other->colliderName == "mesa") {
			if (pelotas[id]->position->GetZ() > 0) {
				if (pelotas[id]->estado == -2) pelotas[id]->estado = -1; //La toco el jugadpr 2 y toco la mesa del jugador 1
			} else {
				if (pelotas[id]->estado == 2) pelotas[id]->estado = 1; //La toco el jugadpr 1 y toco la mesa del jugador 2
			}
		}else if (other->colliderName == "estadio") {
			if (pelotas[id]->estado == 2) pelotas[id]->marcador->IncJ2(); //La toco el jugador 1 y se cayo
			if (pelotas[id]->estado == 1) pelotas[id]->marcador->IncJ1(); //La toco el jugadpr 1 y toco la mesa del jugador 2
			if (pelotas[id]->estado == -1) pelotas[id]->marcador->IncJ2(); //La toco el jugadpr 2 y toco la mesa del jugador 1
			if (pelotas[id]->estado == -2) pelotas[id]->marcador->IncJ1(); //La toco el jugador 2 y se cayo

			pelotas[id]->phy.velocity->SetVertices(0, 0, 0);
			pelotas[id]->position->SetVertices(0, 0.8, 0);
			pelotas[id]->phy.AddImpulse(Vertex3(0, 0, -1.5));
			pelotas[id]->estado = 2;
		}
	}

	void Update() {

	}

};

struct Jugador {
	Vertex3* position;
	Vertex3* rotation;
	Vertex2 posMouse;
	
	CameraTool Camera;
	GameObject raqueta;
	BoundingBox box;
	int index;

	Jugador() {}
	void Init(int i) {
		index = i;
		//Init
		position = new Vertex3(0, 0.35, index*1.2);
		rotation = new Vertex3(0, 0, 0);
		raqueta.Init(position, rotation, true);
		//Camara
		Camera.Init(60, 0.1, 70);
		Camera.SetPoint(Vertex3(0, 1, index*2.4), Vertex3(0, 1, index*2.5), Vertex2(((index == 1) ? 90 : 270), 90));
		//Raqueta OBJ
		TransformMatrix matrix;
		matrix.Identity(); matrix.RotateY(90 * index); matrix.RotateZ(-90); matrix.Scale(0.1, 0.1, 0.1);
		ReadModelOBJ::readOBJ(&raqueta, -1, "Raqueta.obj"); //Las meshes se crean solitas por lo mtllibs
		matrix.MultGameObject(&raqueta);
		//Box
		string name = "J";
		name += ((index == 1) ? "1" : "2");
		box.Init(name, position, rotation, &raqueta, NULL, 0, NULL);
	}
	
	void Kesys(bool up, bool down, bool left, bool right) {
		if (up) posMouse.SetY(posMouse.GetY() + 0.05);
		if (down) posMouse.SetY(posMouse.GetY() - 0.05);
		if (left) posMouse.SetX(posMouse.GetX() - 0.05 * index);
		if (right) posMouse.SetX(posMouse.GetX() + 0.05 * index);
		if (posMouse.GetX() < -0.7) posMouse.SetX(-0.7);
		if (posMouse.GetX() > 0.7) posMouse.SetX(0.7);
		if (posMouse.GetY() < 0) posMouse.SetY(0);
		if (posMouse.GetY() > 0.5) posMouse.SetY(0.5);
		position->SetVertices(posMouse.GetX(), 0.35 + posMouse.GetY(), index*1.2);
	}
	void KesysRot(bool PitchMas, bool PitchMenos, bool RollMas, bool RollMenos, bool YawMas, bool YawMenos) {
		if (PitchMas) rotation->SetX(rotation->GetX() + 10);
		if (PitchMenos) rotation->SetX(rotation->GetX() - 10);
		if (YawMas) rotation->SetY(rotation->GetY() + 2);
		if (YawMenos) rotation->SetY(rotation->GetY() - 2);
		if (RollMas) rotation->SetZ(rotation->GetZ() + 10);
		if (RollMenos) rotation->SetZ(rotation->GetZ() - 10);
	}
	
	void Reshape() {
		Camera.Set((index==1)? 0 : 1, 0, 2, 1); //Camara
	}

	void Update() {
		if (rotation->GetY() == 0) {
			raqueta.material[0].Diffuse(1, 0.2, 0.2);
		} else if (rotation->GetY() > 0) {
			raqueta.material[0].Diffuse(0.2, 1, 0.2);
		} else if (rotation->GetY() < 0) {
			raqueta.material[0].Diffuse(0.2, 0.2, 1);
		}
	}
};

struct Estadio{
	Vertex3* position;
	Vertex3* rotation;

	GameObject stadium;
	GameObject boxGO;
	BoundingBox box[6];

	Estadio() {}

	void Init() {
		//Pos and Rot
		position = new Vertex3(0, 0, 0);
		rotation = new Vertex3(0, 0, 0);
		//Transform
		TransformMatrix matrix;
		//Estadio
		stadium.Init(position, rotation, true);
		matrix.Identity(); matrix.Scale(0.03, 0.03, 0.03); matrix.Translated(0, -5.381100, 0);
		ReadModelOBJ::readOBJ(&stadium, -1, "estadio.obj"); //Las meshes se crean solitas por lo mtllibs
		matrix.MultGameObject(&stadium);
		//Box
		boxGO.Init(position, rotation, false);
		BasicShapesTool::MakeShape(&boxGO, 0, Shape::Cube, 3, 0.5, 5, 0); //Environment
		matrix.Identity(); matrix.Translated(0, -0.25, 0);
		matrix.MultGameObjectMesh(&boxGO, 0);
		BasicShapesTool::MakeShape(&boxGO, 1, Shape::Cube, 3, 0.5, 5, 0);
		matrix.Identity(); matrix.Translated(0, 2.25, 0);
		matrix.MultGameObjectMesh(&boxGO, 1);
		BasicShapesTool::MakeShape(&boxGO, 2, Shape::Cube, 2, 2, 0.5, 0);
		matrix.Identity(); matrix.Translated(0, 1, 2.25);
		matrix.MultGameObjectMesh(&boxGO, 2);
		BasicShapesTool::MakeShape(&boxGO, 3, Shape::Cube, 2, 2, 0.5, 0);
		matrix.Identity(); matrix.Translated(0, 1, -2.25);
		matrix.MultGameObjectMesh(&boxGO, 3);
		BasicShapesTool::MakeShape(&boxGO, 4, Shape::Cube, 0.5, 2, 5, 0);
		matrix.Identity(); matrix.Translated(1.25, 1, 0);
		matrix.MultGameObjectMesh(&boxGO, 4);
		BasicShapesTool::MakeShape(&boxGO, 5, Shape::Cube, 0.5, 2, 5, 0);
		matrix.Identity(); matrix.Translated(-1.25, 1, 0);
		matrix.MultGameObjectMesh(&boxGO, 5);
		//Boxes
		box[0].Init("estadio", position, rotation, &boxGO, 0, NULL, 0, NULL);
		box[1].Init("estadio", position, rotation, &boxGO, 1, NULL, 1, NULL);
		box[2].Init("estadio", position, rotation, &boxGO, 2, NULL, 2, NULL);
		box[3].Init("estadio", position, rotation, &boxGO, 3, NULL, 3, NULL);
		box[4].Init("estadio", position, rotation, &boxGO, 4, NULL, 4, NULL);
		box[5].Init("estadio", position, rotation, &boxGO, 5, NULL, 5, NULL);

	}

	void Update() {

	}

};

struct MesaPing {
	Vertex3* position;
	Vertex3* rotation;

	GameObject mesaPing;
	GameObject boxGO;
	BoundingBox box;

	MesaPing() {}

	void Init() {
		//Pos and Rot
		position = new Vertex3(0, 0, 0);
		rotation = new Vertex3(0, 0, 0);
		//Transform
		TransformMatrix matrix;
		//Mesa
		mesaPing.Init(position, rotation, true);
		matrix.Identity(); matrix.Translated(0, -0.1, 0); matrix.RotateY(90); matrix.Scale(0.08, 0.08, 0.08);
		ReadModelOBJ::readOBJ(&mesaPing, -1, "PingPongTable.obj"); //Las meshes se crean solitas por lo mtllibs
		matrix.MultGameObject(&mesaPing);
		//Box
		boxGO.Init(position, rotation, false);
		BasicShapesTool::MakeShape(&boxGO, 0, Shape::Cube, 1.22, 0.508, 2.2, 0); //Mesa
		matrix.Identity(); matrix.Translated(0, 0.254, 0);
		matrix.MultGameObjectMesh(&boxGO, 0);
		BasicShapesTool::MakeShape(&boxGO, 1, Shape::Cube, 1.22, 0.08, 0.01, 0); //Red
		matrix.Identity(); matrix.Translated(0, 0.548, 0);
		matrix.MultGameObjectMesh(&boxGO, 1);
		//Boxes
		box.Init("mesa", position, rotation, &boxGO, 0, NULL, 0, NULL);
		//box[1].Init("mesa", position, rotation, &boxGO, 1, NULL, 1, OnCollision); 
	}

	void Update() {

	}
};
