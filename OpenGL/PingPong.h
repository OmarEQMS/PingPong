#pragma once
//Todo debe tener Init y Update

struct Jugador {
	CameraTool Camera;
	GameObject raqueta;
	Vertex2 pos;
	int index;

	Jugador() {}
	void Init(int i) {
		index = i;
		raqueta.Init("J" + i);
		//Camara
		Camera.Init(60, 0.1, 70);
		Camera.SetPoint(Vertex3(0, 1, index*2.4), Vertex3(0, 1, index*2.5), Vertex2(((index == 1) ? 90 : 270), 90));
		//Raqueta OBJ
		TransformMatrix matrix;
		matrix.Identity(); matrix.RotateY(90 * index); matrix.RotateZ(-90); matrix.Scale(0.1, 0.1, 0.1);
		raqueta.position->SetVertices(0, 0.35, index*1.2);
		ReadModelOBJ::readOBJ(&raqueta, -1, "Raqueta.obj"); //Las meshes se crean solitas por lo mtllibs
		matrix.MultGameObject(&raqueta);
	}
	
	void Kesys(bool up, bool down, bool left, bool right) {
		if (up) pos.SetY(pos.GetY() + 0.05);
		if (down) pos.SetY(pos.GetY() - 0.05);
		if (left) pos.SetX(pos.GetX() - 0.05 * index);
		if (right) pos.SetX(pos.GetX() + 0.05 * index);
		if (pos.GetX() < -0.7) pos.SetX(-0.7);
		if (pos.GetX() > 0.7) pos.SetX(0.7);
		if (pos.GetY() < 0) pos.SetY(0);
		if (pos.GetY() > 0.5) pos.SetY(0.5);
		raqueta.position->SetVertices(pos.GetX(), 0.35 + pos.GetY(), index*1.2);
	}
	void KesysRot(bool PitchMas, bool PitchMenos, bool RollMas, bool RollMenos) {
		if (PitchMas) raqueta.rotation->SetX(raqueta.rotation->GetX() + 1);
		if (PitchMenos) raqueta.rotation->SetX(raqueta.rotation->GetX() - 1);
		if (RollMas) raqueta.rotation->SetZ(raqueta.rotation->GetZ() + 1 * index);
		if (RollMenos) raqueta.rotation->SetZ(raqueta.rotation->GetZ() - 1 * index);
	}
	
	void Update() {

	}
};

struct Marcador {
	GameObject marcador;
	Texture numeros[11];
	int jugador1, jugador2;

	Marcador() {}
	void Init(){
		//Init
		marcador.Init("marcador");
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
		//Texture
		int index;
		for (int i = 0; i < 5; i++) {
			marcador.pushTexture(index);
			marcador.pushMesh(-1, index);
		}
		//Meshes
		marcador.position->SetVertices(2.65, 1.7, 0);
		BasicShapesTool::MakeShape(&marcador, 0, Shape::Quad, 0.5, 0.5, 0.5, 1);
		BasicShapesTool::MakeShape(&marcador, 1, Shape::Quad, 0.5, 0.5, 0.5, 1);
		BasicShapesTool::MakeShape(&marcador, 2, Shape::Quad, 0.5, 0.5, 0.5, 1);
		BasicShapesTool::MakeShape(&marcador, 3, Shape::Quad, 0.5, 0.5, 0.5, 1);
		BasicShapesTool::MakeShape(&marcador, 4, Shape::Quad, 0.5, 0.5, 0.5, 1);
		TransformMatrix matrix;
		matrix.Identity(); matrix.Translated(0, 0, -1); matrix.RotateZ(-90);
		matrix.MultGameObjectMesh(&marcador, 0);
		matrix.Identity(); matrix.Translated(0, 0, -0.5); matrix.RotateZ(-90);
		matrix.MultGameObjectMesh(&marcador, 1);
		matrix.Identity(); matrix.Translated(0, 0, 0); matrix.RotateZ(-90);
		matrix.MultGameObjectMesh(&marcador, 2);
		matrix.Identity(); matrix.Translated(0, 0, 0.5); matrix.RotateZ(-90);
		matrix.MultGameObjectMesh(&marcador, 3);
		matrix.Identity(); matrix.Translated(0, 0, 1); matrix.RotateZ(-90); 
		matrix.MultGameObjectMesh(&marcador, 4);
		//Set 0-0
		jugador1 = 0; jugador2 = 0;			
		Update();
	}
	
	void Update() {
		marcador.textura[marcador.meshes[0].textura] = numeros[(jugador1 / 10)];
		marcador.textura[marcador.meshes[1].textura] = numeros[(jugador1 % 10)];
		marcador.textura[marcador.meshes[2].textura] = numeros[10];
		marcador.textura[marcador.meshes[3].textura] = numeros[(jugador2 / 10)];
		marcador.textura[marcador.meshes[4].textura] = numeros[(jugador2 % 10)];
	}

	//TODO
};