#pragma once
#define _USE_MATH_DEFINES
#include <Windows.h>
#include <GL/gl.h>
#include "glut.h"
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <cstdio>
#include <ctime>
using namespace std;

typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char uchar;

//ReferenceFrame
struct ReferenceFrame {
	static void drawAxis(double length) { // Draw a z-axis with a cone at the end - From CG using OpenGL - Hill and Kelley
		glPushMatrix();
		glBegin(GL_LINES); // Draw line
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, length);
		glEnd();
		glTranslated(0.0, 0.0, length - 0.2); // Draw cone
		glutSolidCone(0.04, 0.2, 12, 9);
		glPopMatrix();
	};
	static void DrawReferenceFrame() {
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

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
	};
};

//Color
//TODO - Make Color part of Light and Material
struct ColorRGB {
public:
	uchar r, g, b;
	ColorRGB() { r = g = b = 0; }
	ColorRGB(ColorRGB& rgb) { r = rgb.r; g = rgb.g; b = rgb.b; }
	ColorRGB(uchar rr, uchar gg, uchar bb) { r = rr; g = gg; b = bb; }
	void Set(uchar rr, uchar gg, uchar bb) { r = rr; g = gg; b = bb; }
};

//Vertex
struct Vertex3 {
private:
	double vertices[3] = { 0,0,0 };

public:
	static Vertex3 zero;

	Vertex3() {
		vertices[0] = 0; vertices[1] = 0; vertices[2] = 0;
	}
	Vertex3(double X, double Y, double Z) {
		vertices[0] = X; vertices[1] = Y; vertices[2] = Z;
	}

	double* GetVertices() { return vertices; }
	void SetVertices(double x, double y, double z) { SetX(x); SetY(y); SetZ(z); }

	double GetX() { return vertices[0]; }
	void SetX(double val) { vertices[0] = val; }
	double GetY() { return vertices[1]; }
	void SetY(double val) { vertices[1] = val; }
	double GetZ() { return vertices[2]; }
	void SetZ(double val) { vertices[2] = val; }

	//Extras
	void Unitario() {
		double M = Magnitud();
		SetX(GetX() / M);
		SetY(GetY() / M);
		SetZ(GetZ() / M);
	}
	double Magnitud() {
		return sqrt(pow(GetX(), 2) + pow(GetY(), 2) + pow(GetZ(), 2));
	}
	static double Distance(Vertex3& vec1, Vertex3& vec2) {
		return sqrt(pow(vec1.GetX() - vec2.GetX(), 2) + pow(vec1.GetY() - vec2.GetY(), 2) + pow(vec1.GetZ() - vec2.GetZ(), 2));
	}

	//Operaciones
	Vertex3 operator+ (Vertex3 &vec) {
		return Vertex3(GetX() + vec.GetX(), GetY() + vec.GetY(), GetZ() + vec.GetZ());
	}
	Vertex3 operator- (Vertex3 &vec) {
		return Vertex3(GetX() - vec.GetX(), GetY() - vec.GetY(), GetZ() - vec.GetZ());
	}
	Vertex3 operator-() {
		return Vertex3(-GetX(), -GetY(), -GetZ());
	}
	Vertex3 operator/ (double const &val) {
		return Vertex3(GetX() / val, GetY() / val, GetZ() / val);
	}
	Vertex3 operator* (double const &val) {
		return Vertex3(GetX() * val, GetY() * val, GetZ() * val);
	}
	// * = Cross Product
	Vertex3 operator* (Vertex3 &vec) {
		return Vertex3((GetY()*vec.GetZ() - GetZ()*vec.GetY()), -(GetX()*vec.GetZ() - GetZ()*vec.GetX()), (GetX()*vec.GetY() - GetY()*vec.GetX()));
	}
	// & = dot Product
	double operator& (Vertex3 &vec) {
		return ((GetX()*vec.GetX()) + (GetY()*vec.GetY()) + (GetZ()*vec.GetZ()));
	}
	// Each term for each
	Vertex3 operator| (Vertex3 &vec) {
		return Vertex3((GetX()*vec.GetX()), (GetY()*vec.GetY()), (GetZ()*vec.GetZ()));
	}

	//Comparaciones
	bool operator==(Vertex3& other) {
		if (GetX() == other.GetX() && GetY() == other.GetY() && GetZ() == other.GetZ()) return true;
		return false;
	}
	bool operator!=(Vertex3& other) {
		if (GetX() == other.GetX() && GetY() == other.GetY() && GetZ() == other.GetZ()) return false;
		return true;
	}

};
struct Vertex2 {
private:
	double vertices[2] = { 0,0 };
public:
	static Vertex2 zero;

	Vertex2() {
		vertices[0] = 0; vertices[1] = 0;
	}
	Vertex2(double X, double Y) {
		vertices[0] = X; vertices[1] = Y;
	}

	double* GetVertices() { return vertices; }
	void SetVertices(double x, double y) { SetX(x); SetY(y); }

	double GetX() { return vertices[0]; }
	void SetX(double val) { vertices[0] = val; }
	double GetY() { return vertices[1]; }
	void SetY(double val) { vertices[1] = val; }

	//Extras
	void Unitario() {
		double M = Magnitud();
		SetX(GetX() / M);
		SetY(GetY() / M);
	}
	double Magnitud() {
		return sqrt(pow(GetX(), 2) + pow(GetY(), 2));
	}

	//Operaciones
	Vertex2 operator+ (Vertex2 &vec) {
		return Vertex2(GetX() + vec.GetX(), GetY() + vec.GetY());
	}
	Vertex2 operator- (Vertex2 &vec) {
		return Vertex2(GetX() - vec.GetX(), GetY() - vec.GetY());
	}
	Vertex2 operator/ (double const &val) {
		return Vertex2(GetX() / val, GetY() / val);
	}
	Vertex2 operator* (double const &val) {
		return Vertex2(GetX() * val, GetY() * val);
	}
	double operator* (Vertex2 &vec) {
		return (GetX()*vec.GetY() - GetY()*vec.GetX());
	}

	//Comparaciones
	bool operator==(Vertex2& other) {
		if (GetX() == other.GetX() && GetY() == other.GetY()) return true;
		return false;
	}
	bool operator!=(Vertex2& other) {
		if (GetX() == other.GetX() && GetY() == other.GetY()) return false;
		return true;
	}
};

//Fog
struct Fog {
public:

	static void Enable(GLint fogMode, float* fogColor, float density, float start, float end) {
		glEnable(GL_FOG);
		glFogi(GL_FOG_MODE, fogMode); // GL_EXP, GL_EXP2, GL_LINEAR
		glFogfv(GL_FOG_COLOR, fogColor); // RGBA color
		glFogf(GL_FOG_DENSITY, density); // 0.0 to 1.0
		glFogf(GL_FOG_START, start);
		glFogf(GL_FOG_END, end);
		// calculate by pixel (GL_NICEST) or by vertex (GL_FASTEST) or GL_DONT_CARE
		glHint(GL_FOG_HINT, GL_DONT_CARE);
	}
	static void Disable() {
		glDisable(GL_FOG);
	}

};

//Lights and Materials
struct Light {
public:
	static vector<Light*> luces;
private:	
	float position[4] = { 0,0,0,1 }; //4 - (1)Punto o (0)Vector
	float ambient[4] = { 0,0,0,1 }; //4 - Que tanto se ve afectado el ambiente por esa luz
	float specular[4] = { 0,0,0,1 };
	float diffuse[4] = { 0,0,0,1 };	
	float atenuation[3] = { 1, 0, 0 };  //Atenuation - constant, linear, cuadratic
	float direction[3] = { 0,0,-1 }; //CutOff
	float angle = 180;
	float exponent = 0;

	GLenum light;
	bool enable = true;
	bool automatico;

public:
	Light() {
		automatico = false;
	}
	Light(GLenum luz, bool autom) {
		luces.push_back(this);
		automatico = autom;
		light = luz;
	}

	static void SetAll() {
		for (int i = 0; i < luces.size(); i++) {
			if (luces[i]->automatico) {
				luces[i]->Set();
			}
		}
	}
	void Set() {
		glLightfv(light, GL_AMBIENT, ambient); //Color ambiente
		glLightfv(light, GL_SPECULAR, specular); //Color shine	
		glLightfv(light, GL_DIFFUSE, diffuse); //Color object
		glLightfv(light, GL_POSITION, position);
		//Attenuation
		glLightf(light, GL_CONSTANT_ATTENUATION, atenuation[0]);
		glLightf(light, GL_LINEAR_ATTENUATION, atenuation[1]);
		glLightf(light, GL_QUADRATIC_ATTENUATION, atenuation[2]);
		//Cutoff
		glLightf(light, GL_SPOT_CUTOFF, angle);
		glLightf(light, GL_SPOT_EXPONENT, exponent);
		glLightfv(light, GL_SPOT_DIRECTION, direction);
		if (enable) glEnable(light);
		glEnable(GL_LIGHTING);
	}
	
	void Draw() {
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glTranslated(position[0], position[1], position[2]);
		glColor3fv(diffuse);
		glutWireSphere(0.15, 3, 2);
	}

	Vertex3 GetPosition() { return Vertex3((double)position[0], (double)position[1], (double)position[2]); }
	Vertex3 GetAmbient() { return Vertex3((double)ambient[0], (double)ambient[1], (double)ambient[2]); }
	Vertex3 GetDiffuse() { return Vertex3((double)diffuse[0], (double)diffuse[1], (double)diffuse[2]); }
	Vertex3 GetSpecular() { return Vertex3((double)specular[0], (double)specular[1], (double)specular[2]); }

	void SetEnable(bool en) { enable = en; if (en) glEnable(light); else glDisable(light); }
	bool GetEnable() { return enable; }
	
	void Assign(float amb, float diff, float spec) {
		Ambient(amb,amb,amb);
		Diffuse(diff,diff,diff);
		Specular(spec,spec,spec);
		Set();
	}
	void Assign(float* amb, float* diff, float* spec, float* pos) {
		if (amb != NULL) {
			ambient[0] = amb[0];
			ambient[1] = amb[1];
			ambient[2] = amb[2];
		}
		if (diff != NULL) {
			diffuse[0] = diff[0];
			diffuse[1] = diff[1];
			diffuse[2] = diff[2];
		}
		if (spec != NULL) {
			specular[0] = spec[0];
			specular[1] = spec[1];
			specular[2] = spec[2];
		}
		if (pos != NULL) {
			position[0] = pos[0];
			position[1] = pos[1];
			position[2] = pos[2];
		}
		Set();
	}

	void DisableAttenuation() {
		atenuation[0] = 1;
		atenuation[1] = 0;
		atenuation[2] = 0;
		Set();
	}
	void SetAttenuation(float cons, float lin, float cuad) {
		atenuation[0] = cons;
		atenuation[1] = lin;
		atenuation[2] = cuad;
		Set();
	}
	void DisableCutoff() {
		direction[0] = 0;
		direction[1] = 0;
		direction[2] = -1;
		angle = 180;
		exponent = 0;
	}
	void SetCutoff(float* dir, float ang, float exp) {
		direction[0] = dir[0];
		direction[1] = dir[1];
		direction[2] = dir[2];
		angle = ang;
		exponent = exp;
	}

	void PointOrVector(int pv){ //1 = punto, 0 = vector
		position[3] = pv;
	}
	void Position(float x, float y, float z) {
		position[0] = x;
		position[1] = y;
		position[2] = z;
		Set();
	}
	void Diffuse(float r, float g, float b) {
		diffuse[0] = r;
		diffuse[1] = g;
		diffuse[2] = b;
		Set();
	}
	void Specular(float r, float g, float b) {
		specular[0] = r;
		specular[1] = g;
		specular[2] = b;
		Set();
	}
	void Ambient(float r, float g, float b) {
		ambient[0] = r;
		ambient[1] = g;
		ambient[2] = b;
		Set();
	}

};
struct Material {
private:
	float ambient[4] = { 0,0,0,1 }; //4 - Que tanto se ve afectado el objeto por la luz
	float diffuse[4] = { 0,0,0,1 };
	float specular[4] = { 0,0,0,1 };
	float emission[4] = { 0,0,0,1 };
	float shinnes = 0;

public:
	Material() {}

	void Set() {
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient); //RGBA de la luz ambiental
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse); //RGBA de diffuse, el material
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular); //RGBA de specular el shinny
		glMaterialf(GL_FRONT, GL_SHININESS, shinnes); //specular exponent [0, 128] - high:100.0f  low:5.0  no:0.0
	}

	Vertex3 GetAmbient() { return Vertex3((double)ambient[0], (double)ambient[1], (double)ambient[2]); }
	Vertex3 GetDiffuse() { return Vertex3((double)diffuse[0], (double)diffuse[1], (double)diffuse[2]); }
	Vertex3 GetSpecular() { return Vertex3((double)specular[0], (double)specular[1], (double)specular[2]); }
	double GetShinnes() { return shinnes; }

	void Assign(float* emi, float* amb, float* diff, float* spec, float* shi) {
		if (emi != NULL) {
			emission[0] = emi[0];
			emission[1] = emi[1];
			emission[2] = emi[2];
		}
		if (amb != NULL) {
			ambient[0] = amb[0];
			ambient[1] = amb[1];
			ambient[2] = amb[2];
		}
		if (diff != NULL) {
			diffuse[0] = diff[0];
			diffuse[1] = diff[1];
			diffuse[2] = diff[2];
		}
		if (spec != NULL) {
			specular[0] = spec[0];
			specular[1] = spec[1];
			specular[2] = spec[2];
		}
		if (shi != NULL) {
			shinnes = *shi;
		}
		Set();
	}

	void Emission(float r, float g, float b) {
		emission[0] = r;
		emission[1] = g;
		emission[2] = b;
	}
	void Ambient(float r, float g, float b) {
		ambient[0] = r;
		ambient[1] = g;
		ambient[2] = b;
	}
	void Diffuse(float r, float g, float b) {
		diffuse[0] = r;
		diffuse[1] = g;
		diffuse[2] = b;
	}
	void Specular(float r, float g, float b) {
		specular[0] = r;
		specular[1] = g;
		specular[2] = b;
	}
	void Shinnes(float shi) {
		shinnes = shi;
	}

}; 

//Face and Mesh (indices empiezan en 1)
struct Face {
public:
	vector<int> vn;
	vector<int> vt;
	vector<int> v;
	bool visible = true;

	void setV(int* ver, int n) {
		for (int i = 0; i < n; i++) {
			v.push_back(ver[i]);
		}
	}
	void setVT(int* ver, int n) {
		for (int i = 0; i < n; i++) {
			vt.push_back(ver[i]);
		}
	}
	void setVN(int* ver, int n) {
		for (int i = 0; i < n; i++) {
			vn.push_back(ver[i]);
		}
	}
	int count() { return v.size(); };
	
};
struct Mesh {
public:
	vector<int> faces;
	int material;
	int textura;
	bool visible = true;

	Mesh() {
		material = -1;
		textura = -1;
	}
	Mesh(int mat, int tex) {
		material = mat;
		textura = tex;
	}
	void setF(int* f, int n) {
		for (int i = 0; i < n; i++) {
			faces.push_back(f[i]);
		}
	}
	
};

//Shapes
enum Shape { Cylinder, Sphere, Cube, Icosaedro, Quad };

//BitMap
struct Texture {
public:
	int nRows, nCols;
	ColorRGB *pixel;
	GLuint name;

	//SetTexture
	void SetTexture() {
		// Bind 2D textures, based on generated name
		glGenTextures(1, &name);
		glBindTexture(GL_TEXTURE_2D, name);
		// Set wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//Set interpolation
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR  or GL_NEAREST
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//Aligment of pixels in array
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
		// Generate 2D texture image - use class member variables: nCols, nRows, pixel
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nCols, nRows, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	}
	//Cada vez que lo pintes
	void DisplayTexture(bool mat) {
		int mode = mat ? GL_MODULATE : GL_DECAL;
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode); // Set texture mode (GL_DECAL, GL_ADD, GL_MODULATE, GL_BLEND)
		glBindTexture(GL_TEXTURE_2D, name); // Bind texture	
	}

	void operator= (Texture const &text) {
		nRows = text.nRows;
		nCols = text.nCols;
		pixel = text.pixel;
		name = text.name;
		SetTexture();
	}
		
};

struct ReadBitMap {
public:
	static void ReadBMPFile(Texture* textura, string fname) {
		fstream inf;
		// Read into memory an mRGB image from an uncompressed BMP file.
		// return 0 on failure, 1 on success
		inf.open(fname, ios::in | ios::binary); //read binary char's
		if (!inf) { cout << "Can't open file: " << fname << endl; return; }
		int k, row, col, numPadBytes, nBytesInRow;
		// read the file header information
		char ch1 = getChar(inf);	//type: always 'BM'
		char ch2 = getChar(inf);
		ulong fileSize = getLong(inf);
		ushort reserved1 = getShort(inf);    // always 0
		ushort reserved2 = getShort(inf);     // always 0
		ulong offBits = getLong(inf); // offset to image - unreliable
		ulong headerSize = getLong(inf);     // always 40
		ulong numCols = getLong(inf); // number of columns in image
		ulong numRows = getLong(inf); // number of rows in image
		ushort planes = getShort(inf);      // always 1
		ushort bitsPerPixel = getShort(inf);    //8 or 24; allow 24 here
		ulong compression = getLong(inf);      // must be 0 for uncompressed
		ulong imageSize = getLong(inf);       // total bytes in image
		ulong xPels = getLong(inf);    // always 0
		ulong yPels = getLong(inf);    // always 0
		ulong numLUTentries = getLong(inf);    // 256 for 8 bit, otherwise 0
		ulong impColors = getLong(inf);       // always 0
		if (bitsPerPixel != 24){ // error - must be a 24 bit uncompressed image
			cout << "Not a 24 bit/pixelimage, or is compressed!" << endl; inf.close(); return;
		}
		//add bytes at end of each row so total # is a multiple of 4
		// round up 3*numCols to next mult. of 4
		nBytesInRow = ((3 * numCols + 3) / 4) * 4;
		numPadBytes = nBytesInRow - 3 * numCols; // need this many
		textura->nRows = numRows; // set class's data members
		textura->nCols = numCols;
		textura->pixel = new ColorRGB[textura->nRows * textura->nCols]; //make space for array
		if (!textura->pixel) { cout << "Out of Memory" << endl; return; }// out of memory!
		long count = 0;
		char dum;
		for (row = 0; row < textura->nRows; row++) { // read pixel values
			for (col = 0; col < textura->nCols; col++) {
				char b = getChar(inf), g = getChar(inf), r = getChar(inf);
				textura->pixel[count].r = r; //place them in colors
				textura->pixel[count].g = g;
				textura->pixel[count++].b = b;
			}
			for (k = 0; k < numPadBytes; k++) { inf >> dum; } //skip pad bytes at row's end
		}
		textura->SetTexture(); //SetTexture
		inf.close(); 
		return; //Success
	}
	static char getChar(fstream& inf) { //2 bytes
		//BMP format uses little-endian integer types
		// get a 1-byte integer stored in little-endian form
		char ip;
		inf.get(ip);
		return ip;
	}
	static ushort getShort(fstream& inf) { //2 bytes
		//BMP format uses little-endian integer types
		// get a 2-byte integer stored in little-endian form
		char ic;
		ushort ip;
		inf.get(ic); ip = ic;  //first byte is little one
		inf.get(ic);  ip |= ((ushort)ic << 8); // or in high order byte
		return ip;
	}
	static ulong getLong(fstream& inf) { //4 bytes
		//BMP format uses little-endian integer types
		// get a 4-byte integer stored in little-endian form
		ulong ip = 0;
		char ic = 0;
		unsigned char uc = ic;
		inf.get(ic); uc = ic; ip = uc;
		inf.get(ic); uc = ic; ip |= ((ulong)uc << 8);
		inf.get(ic); uc = ic; ip |= ((ulong)uc << 16);
		inf.get(ic); uc = ic; ip |= ((ulong)uc << 24);
		return ip;
	}
};

struct BitMapCreation {
public:
	static void MakeShadedCircle(Texture* text, int* color) {
		float r, val, size = 256;
		text->nRows = size; // set class's data members
		text->nCols = size;
		text->pixel = new ColorRGB[size * size]; //make space for array
		long count = 0;
		for (int row = 0; row < size; row++) { // read pixel values
			for (int col = 0; col < size; col++) {
				r = sqrt((float)pow(col - size /2, 2) + (float)pow(row - size /2, 2)) / size;
				if (r<0.3) val = 1 - r / 0.3;
				else val = 0.2;
				text->pixel[count].r = val*255.0*color[0];
				text->pixel[count].g = val*255.0*color[1];
				text->pixel[count].b = val*255.0*color[2];
				count++;
			}
		}
		text->SetTexture();
	}
	static void MakeCheckboard(Texture* text, int* color) {
		int x,y,val,n=8,size = 256;
		text->nRows = size; // set class's data members
		text->nCols = size;
		text->pixel = new ColorRGB[size * size]; //make space for array
		long count = 0;
		for (int row = 0; row < size; row++) { // read pixel values
			for (int col = 0; col < size; col++) {
				x = row / (size / n);
				y = col / (size / n);
				val = (int)(x + y) % 2;
				text->pixel[count].r = val*255.0*color[0];
				text->pixel[count].g = val*255.0*color[1];
				text->pixel[count].b = val*255.0*color[2];
				count++;
			}
		}
		text->SetTexture();
	}
	static void MakeRandom(Texture* text) {
		int x, y, n = 8, size = 256;
		vector<int> val; for(int i = 0; i < n*n; i++) val.push_back(rand() % 8);
		text->nRows = size; // set class's data members
		text->nCols = size;
		text->pixel = new ColorRGB[size * size]; //make space for array
		long count = 0;
		for (int row = 0; row < size; row++) { // read pixel values
			for (int col = 0; col < size; col++) {
				x = row / (size / n);
				y = col / (size / n);
				text->pixel[count].r = (double)(val[y*n + x]%2) *255.0;
				text->pixel[count].g = (double)((val[y*n + x]/2)%2) *255.0;
				text->pixel[count].b = (double)((val[y*n + x]/4)%2) *255.0;
				count++;
			}
		}
		text->SetTexture();
	}
};

//Timer
//https://www.fluentcpp.com/2018/12/28/timer-cpp/
struct Timer {
private:
	bool clear = false;
public:
	template<typename Function> void setTimeout(Function function, int delay) {
		this->clear = false;
		thread t([=]() {
			if (this->clear) return;
			this_thread::sleep_for(chrono::milliseconds(delay));
			if (this->clear) return;
			function();
		});
		t.detach();
	}
	template<typename Function> void setInterval(Function function, int interval) {
		this->clear = false;
		thread t([=]() {
			while (true) {
				if (this->clear) return;
				this_thread::sleep_for(std::chrono::milliseconds(interval));
				if (this->clear) return;
				function();
			}
		});
		t.detach();
	}
	void stop() {
		this->clear = true;
	}
};

//INPUT //TODO
class Input {
	
};

//GameObject
//TODO - Delete Objects and Meshez (Clean Vectors)
struct  GameObject {
public:
	static vector<GameObject*> GameObjects;
public:
	int index;
	Vertex3* position;
	Vertex3* rotation;
	vector<Vertex3> vertexes; //inician desde 0
	vector<Vertex3> vertexNormals;
	vector<Vertex2> vertexTexture;	
	vector<Mesh> meshes;
	vector<Face> faces;
	vector<Texture>	textura;
	vector<Material> material;
	bool visible = true;
	bool automatico;

	GameObject() {
		index = GameObjects.size();
		GameObjects.push_back(this);
	}
	void Init() {
		position = new Vertex3();
		rotation = new Vertex3();
		automatico = true;
	}
	void Init(bool autom) {
		position = new Vertex3();
		rotation = new Vertex3();
		automatico = autom;
	}
	void Init(Vertex3* pos, Vertex3* rot, bool autom) {
		position = pos;
		rotation = rot;
		automatico = autom;
	}

	void setVertexes(Vertex3* ver, int n) {
		for (int i = 0; i < n; i++) {
			vertexes.push_back(ver[i]);
		}
	}
	void setVertexesTexture(Vertex2* ver, int n) {
		for (int i = 0; i < n; i++) {
			vertexTexture.push_back(ver[i]);
		}
	}
	void pushFace(Face f, int mesh) {
		if (mesh >= meshes.size()) {
			pushMesh(-1, -1);
		}
		int nuevo = faces.size();
		faces.push_back(f);
		meshes[mesh].faces.push_back(nuevo);
	}
	int pushMesh(int mat, int tex) {
		int nuevo = meshes.size();
		meshes.push_back(Mesh(mat, tex));
		return nuevo;
	}
	Material* pushMaterial(int& index) {
		index = material.size();
		material.resize(index + 1);
		return &material[index];
	}
	Texture* pushTexture(int& index) {
		index = textura.size();
		textura.resize(index + 1);
		return &textura[index];
	}

	static void DrawAll() {
		for (int i = 0; i < GameObjects.size(); i++) {
			if (GameObjects[i]->automatico) {
				GameObjects[i]->Draw();
			}
		}
	}

	void Draw() {
		if (visible) {
			glPushMatrix();
			glTranslated(position->GetX(), position->GetY(), position->GetZ());
			glRotated(rotation->GetY(), 0, 1, 0);
			glRotated(rotation->GetX(), 1, 0, 0);
			glRotated(rotation->GetZ(), 0, 0, 1);
			//Iterate Meshes
			for (int i = 0; i < meshes.size(); i++) {
				if (meshes[i].visible) {
					if (meshes[i].textura >= 0) { glEnable(GL_LIGHTING);  glEnable(GL_TEXTURE_2D); textura[meshes[i].textura].DisplayTexture(meshes[i].material >= 0); }
					else { glDisable(GL_TEXTURE_2D); }
					if (meshes[i].material >= 0) { glEnable(GL_LIGHTING); material[meshes[i].material].Set(); }

					for (int j = 0; j < meshes[i].faces.size(); j++) {
						glBegin(GL_POLYGON);
						for (int k = 0; k < faces[meshes[i].faces[j]].count(); k++) {
							glNormal3dv(vertexNormals[faces[meshes[i].faces[j]].vn[k]].GetVertices());
							if (meshes[i].textura >= 0) glTexCoord2dv(vertexTexture[faces[meshes[i].faces[j]].vt[k]].GetVertices());
							glVertex3dv(vertexes[faces[meshes[i].faces[j]].v[k]].GetVertices());
						}
						glEnd();
					}
				}
			}
			glPopMatrix();
		}
	}
	
	void Reset() {
		//Delete Objects
	}
	void ResetMesh() {
		//Delete Mesh from Objects
	}
};

//TransformMatrix
struct TransformMatrix {
private:
	struct Matrix {
	public:
		double matrix[4][4];
		Matrix() {}
		Matrix(double mat[4][4]) {
			for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) matrix[i][j] = mat[i][j];
		};
		void Set(Matrix mat) {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					matrix[i][j] = mat.matrix[i][j];
		}
	};
	vector<Matrix> pila;
	Matrix matrix;

public:
	void PushMatrix() { pila.push_back(*(new Matrix(matrix.matrix))); }
	void PopMatrix() { matrix.Set(pila.back()); pila.pop_back(); }

	void Identity() {
		double identity[4][4] = { { 1,0,0,0 },{ 0,1,0,0 },{ 0,0,1,0 },{ 0,0,0,1 } };
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				matrix.matrix[i][j] = identity[i][j];
	}

	Vertex3 MultVertex(Vertex3 point) {
		double punto[4] = { point.GetX(), point.GetY(), point.GetZ(), 1 };
		double result[4] = { 0,0,0,0 };
		for (int i = 0; i < 4; i++) { //Fila
			for (int j = 0; j < 1; j++) { //Columna
				for (int k = 0; k < 4; k++) {
					result[i] += matrix.matrix[i][k] * punto[k]; //j = 0
				}
			}
		}
		return Vertex3(result[0], result[1], result[2]);
	}
	Vertex3 MultVector(Vertex3 vector) {
		double punto[4] = { vector.GetX(), vector.GetY(), vector.GetZ(), 0 };
		double result[4] = { 0,0,0,0 };
		for (int i = 0; i < 4; i++) { //Fila
			for (int j = 0; j < 1; j++) { //Columna
				for (int k = 0; k < 4; k++) {
					result[i] += matrix.matrix[i][k] * punto[k]; //j = 0
				}
			}
		}
		return Vertex3(result[0], result[1], result[2]);
	}
	void MultGameObject(GameObject* go) {
		for (int i = 0; i < go->vertexes.size(); i++) {
			go->vertexes[i] = MultVertex(go->vertexes[i]);
		}
	}
	void MultGameObjectMesh(GameObject* go, int mesh) {
		vector<int> repeat;
		for (int j = 0; j < go->meshes[mesh].faces.size(); j++) {
			for (int k = 0; k < go->faces[go->meshes[mesh].faces[j]].count(); k++) {
				int v = go->faces[go->meshes[mesh].faces[j]].v[k];
				bool rep = false; for (int l = 0; l < repeat.size(); l++) { if (repeat[l] == v) rep = true; }
				if (!rep) {
					repeat.push_back(v);
					go->vertexes[v] = MultVertex(go->vertexes[v]);
				}
			}
		}
	}

	//Matriz = Renglon * Columna
	//OpenGL usea Pre-Multiplication, yo no se que uso, jejeje
	void MulMatrix(Matrix mat) {
		double result[4][4] = { { 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } };
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					result[i][j] += matrix.matrix[i][k] * mat.matrix[k][j];
				}
			}
		}
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				matrix.matrix[i][j] = result[i][j];
	}

	void Translated(double x, double y, double z) {
		double translate[4][4] = { { 1,0,0,x },{ 0,1,0,y },{ 0,0,1,z },{ 0,0,0,1 } };
		MulMatrix(Matrix(translate));
	}
	void RotateX(double angle) {
		angle = angle * M_PI / 180;
		double rotate[4][4] = { { 1,0,0,0 },{ 0,cos(angle),sin(angle),0 },{ 0,-sin(angle),cos(angle),0 },{ 0,0,0,1 } };
		MulMatrix(Matrix(rotate));
	}
	void RotateY(double angle) {
		angle = angle * M_PI / 180;
		double rotate[4][4] = { { cos(angle),0,sin(angle),0 },{ 0,1,0,0 },{ -sin(angle),0,cos(angle),0 },{ 0,0,0,1 } };
		MulMatrix(Matrix(rotate));
	}
	void RotateZ(double angle) {
		angle = angle * M_PI / 180;
		double rotate[4][4] = { { cos(angle),sin(angle),0,0 },{ -sin(angle),cos(angle),0,0 },{ 0,0,1,0 },{ 0,0,0,1 } };
		MulMatrix(Matrix(rotate));
	}
	void Scale(double x, double y, double z) {
		double scale[4][4] = { { x,0,0,0 },{ 0,y,0,0 },{ 0,0,z,0 },{ 0,0,0,1 } };
		MulMatrix(Matrix(scale));
	}

};

//Physics
//TODO Colision con dos objetos movibles
struct Physics {
public:
	static vector<Physics*> Physicses;
	static Vertex3 globalAcceleration;
	static double dt;
public:
	int index;
	Vertex3* position;
	Vertex3 positionAnt;
	Vertex3* velocity;
	Vertex3* acceleration;
	double masa;

	Physics() {
		index = Physicses.size();
		Physicses.push_back(this);
	}
	void Init(Vertex3* pos, double masa) {
		position = pos;
		this->masa = masa;
		velocity = new Vertex3();
		acceleration = new Vertex3();
	}

	/*
	Impulso
		I = F dt
		F = m a
		I / m = a dt
	Colisiones elasticas, dos objetos movibles
		P = m v
		V1x = ((U1x * M1) + (U2x*M2) - (U1x-U2x)*M2) / (M1 + M2)
		V2x = ((U1x * M1) + (U2x*M2) - (U2x - U1x)*M1) / (M1 + M2
	Movimiento
		xt = xt - 1 + vt dt
		vt = vt - 1 + at dt
		atotal = at + aext + auser
	*/

	void OnCollision(Vertex3 direction, Physics* other) { //Se conserva toda la energia		
		if (other == NULL) { //Velocity = Reflected Velocity if other physics == NULL
			double magnitud = velocity->Magnitud();
			velocity->Unitario();
			direction.Unitario();
			*velocity = (direction * ((-*velocity) & direction) * 2) + *velocity;
			*velocity = *velocity * magnitud;
			//HACK
			*position = positionAnt;
			*position = *position + *velocity * dt;
		} else { //Si el otro tambien tiene phisicas
			// V1x = ((U1x * M1) + (U2x*M2) - (U1x-U2x)*M2) / (M1 + M2)
			// V2x = ((U1x * M1) + (U2x*M2) - (U2x - U1x)*M1) / (M1 + M2
		}
	}

	void AddImpulse(Vertex3 imp) {
		Vertex3 acc = imp / (masa * dt);
		*acceleration = acc;
	}

	static void UpdatePhysics() {
		for (int i = 0; i < Physicses.size(); i++) {
			Physicses[i]->positionAnt = *Physicses[i]->position;
			*Physicses[i]->acceleration = *Physicses[i]->acceleration + globalAcceleration;
			*Physicses[i]->velocity = *Physicses[i]->velocity + *Physicses[i]->acceleration * dt;
			*Physicses[i]->position = *Physicses[i]->position + *Physicses[i]->velocity * dt;	
			*Physicses[i]->acceleration = Vertex3::zero;
		}
	}

};

//BoundingBox Collisions
struct BoundingBox {
public:
	static vector<BoundingBox*> BoundingBoxes;
	static vector<vector<int>> collisionDetection;
public:
	int index;
	string colliderName;
	Physics* physiscs;
	Vertex3* position;
	Vertex3* rotation;
	void (*OnCollision)(int id, BoundingBox* other, Vertex3 direction);
	int localIdentifier;	
	vector<Vertex3> boundingBox; //Para SAT
	vector<Vertex3> normals; //Para Sat
	vector<Vertex3> normalesPlane; //Para detectarCara
	vector<Vertex3> puntosPlane; //Para detectarCara
	Vertex3 centerBox;

	BoundingBox() {		
		index = BoundingBoxes.size();
		BoundingBoxes.push_back(this);
		collisionDetection.resize(collisionDetection.size() + 1);
		for (int i = 0; i < collisionDetection.size(); i++) {
			collisionDetection[i].resize(i, 0);
		}
	}
	void Init(string name, Vertex3* pos, Vertex3* rot, GameObject* go, Physics* phy, int localIdentifier, void(*OnCollision)(int id, BoundingBox* other, Vertex3 direction)) {
		colliderName = name;
		position = pos;
		rotation = rot;
		physiscs = phy;
		this->OnCollision = OnCollision;
		this->localIdentifier = localIdentifier;
		CalculateBoundingBoxGameObject(go);
	}
	void Init(string name, Vertex3* pos, Vertex3* rot, GameObject* go, int mesh, Physics* phy, int id, void(*OnCollision)(int id, BoundingBox* other, Vertex3 direction)) {
		colliderName = name;
		position = pos;
		rotation = rot;
		physiscs = phy;
		this->OnCollision = OnCollision;
		this->localIdentifier = localIdentifier;
		bool meshB = true;
		CalculateBoundingBoxMesh(go, mesh, meshB);
	}

	static void AddCollisionDetection(int indexA, int indexB, bool value) {
		if ((indexA == indexB) || (indexA >= collisionDetection.size()) || (indexB >= collisionDetection.size())) return;
		int prim = (indexA < indexB) ? indexA : indexB;
		int seg = (indexA < indexB) ? indexB : indexA;
		collisionDetection[seg][prim] = value? 1 : 0;
	}
	static void CheckCollisions() {
		TransformMatrix matrixI, matrixJ;
		for (int i = 0; i < collisionDetection.size(); i++) {
			for (int j = 0; j < collisionDetection[i].size(); j++) {
				if (collisionDetection[i][j] == 1) {					
					//BoundingBoxes
					//Itero las normales - Saco la normal de los dos bounding boxes
					//Colapso los puntos - Si en alguna normal no interseccion colision = false;
					//El centro del objeto es position + centerObj y Los puntos del noundin box son position + boundingBox

					//Transform Points
					matrixI.Identity();
					matrixI.Translated(BoundingBoxes[i]->position->GetX(), BoundingBoxes[i]->position->GetY(), BoundingBoxes[i]->position->GetZ());
					matrixI.RotateY(BoundingBoxes[i]->rotation->GetY());
					matrixI.RotateX(-BoundingBoxes[i]->rotation->GetX());
					matrixI.RotateZ(-BoundingBoxes[i]->rotation->GetZ());

					matrixJ.Identity();
					matrixJ.Translated(BoundingBoxes[j]->position->GetX(), BoundingBoxes[j]->position->GetY(), BoundingBoxes[j]->position->GetZ());
					matrixJ.RotateY(BoundingBoxes[j]->rotation->GetY());
					matrixJ.RotateX(-BoundingBoxes[j]->rotation->GetX());
					matrixJ.RotateZ(-BoundingBoxes[j]->rotation->GetZ());

					//SAT Normals Object I
					bool colision = true;
					for (int k = 0; k < BoundingBoxes[i]->normals.size(); k++) {
						if (!colision) break;
						if (!SAT(matrixI, BoundingBoxes[i]->centerBox, BoundingBoxes[i]->normals[k], BoundingBoxes[i]->boundingBox, matrixJ, BoundingBoxes[j]->boundingBox)) {
							colision = false;
						}
					}
					//SAT Normals Object J					
					for (int k = 0; k < BoundingBoxes[j]->normals.size(); k++) {
						if (!colision) break;
						if (!SAT(matrixJ, BoundingBoxes[j]->centerBox, BoundingBoxes[j]->normals[k], BoundingBoxes[j]->boundingBox, matrixI, BoundingBoxes[i]->boundingBox)) {
							colision = false;
						}
					}

					//Si si Interseccion
					if (colision) {
						Vertex3 vectorI, vectorJ; //El vector entra en el objeto
						//Si los dos tienen fisicas, Calculo el vector de colision es A- B y B - A
						if (BoundingBoxes[i]->physiscs != NULL && BoundingBoxes[j]->physiscs != NULL) {
							vectorI = (*BoundingBoxes[i]->position + BoundingBoxes[i]->centerBox) - (*BoundingBoxes[j]->position + BoundingBoxes[j]->centerBox);
							vectorJ = -vectorI;
						}
						//If one doesnot have physics, Vector = Normal vector of colliding face
						else {
							//Debo hacer el caulculo con las normales de I
							if (BoundingBoxes[j]->physiscs != NULL) { 
								vectorJ = BoxPlaneInterseccion(matrixI, BoundingBoxes[i]->centerBox, BoundingBoxes[i]->normalesPlane, BoundingBoxes[i]->puntosPlane, matrixJ, BoundingBoxes[j]->centerBox, *BoundingBoxes[j]->physiscs->velocity);
								vectorI = -vectorJ;
							}
							//Hago el calculo con las normales de J
							else if (BoundingBoxes[i]->physiscs != NULL) {
								vectorI = BoxPlaneInterseccion(matrixJ, BoundingBoxes[j]->centerBox, BoundingBoxes[j]->normalesPlane, BoundingBoxes[j]->puntosPlane, matrixI, BoundingBoxes[i]->centerBox, *BoundingBoxes[i]->physiscs->velocity);
								vectorJ = -vectorI;
							}
							//Como ninguno tiene velocidad, el vector de collision es la diferencia entre sus pocisiones
							else {
								vectorI = (*BoundingBoxes[i]->position + BoundingBoxes[i]->centerBox) - (*BoundingBoxes[j]->position + BoundingBoxes[j]->centerBox);
								vectorJ = -vectorI;
							}
						}

						//Make Sure is Unitary
						vectorI.Unitario();
						vectorJ.Unitario();

						//Physics Collision
						if (BoundingBoxes[i]->physiscs != NULL) BoundingBoxes[i]->physiscs->OnCollision(vectorI, BoundingBoxes[j]->physiscs);
						if (BoundingBoxes[j]->physiscs != NULL) BoundingBoxes[j]->physiscs->OnCollision(vectorJ, BoundingBoxes[i]->physiscs);
						
						//Custom Collisions
						if (BoundingBoxes[i]->OnCollision != NULL) {
							thread onCol(BoundingBoxes[i]->OnCollision, BoundingBoxes[i]->localIdentifier, BoundingBoxes[j], vectorI);
							onCol.detach();
						}
						if (BoundingBoxes[j]->OnCollision != NULL) {
							thread onCol(BoundingBoxes[j]->OnCollision, BoundingBoxes[j]->localIdentifier, BoundingBoxes[i], vectorJ);
							onCol.detach();
						}
					}
				}
			}
		}
	}

	// Separating Axis Theorem
	static bool SAT(TransformMatrix matrixA, Vertex3 centerObjA, Vertex3 normalA, vector<Vertex3> puntosA, TransformMatrix matrixB, vector<Vertex3> puntosB) {
		double minA = DBL_MAX, maxA = -DBL_MAX;
		double minB = DBL_MAX, maxB = -DBL_MAX;
		//(A)
		for (int i = 0; i < puntosA.size(); i++){
			// GetReflected Vector on Axis
			double dotVal = matrixA.MultVector(normalA) & (matrixA.MultVertex(puntosA[i]) - matrixA.MultVertex(centerObjA));
			if (dotVal < minA) minA = dotVal;
			if (dotVal > maxA) maxA = dotVal;
		}
		//(B)		
		for (int i = 0; i < puntosB.size(); i++) {
			// GetReflected Vector on Axis
			double dotVal = matrixA.MultVector(normalA) & (matrixB.MultVertex(puntosB[i]) - matrixA.MultVertex(centerObjA));
			if (dotVal < minB) minB = dotVal;
			if (dotVal > maxB) maxB = dotVal;
		}		
		//Overlaps
		return ((minA <= minB && minB <= maxA) || (minB <= minA && minA <= maxB));
	}

	//BoxPlaneInterseccion
	static Vertex3 BoxPlaneInterseccion(TransformMatrix matrix, Vertex3 centerObj, vector<Vertex3> normalesPlane, vector<Vertex3> puntosPlane, TransformMatrix matrixOther, Vertex3 centerObjOther, Vertex3 velocityOther) {
		Vertex3 direction = velocityOther; direction.Unitario(); //La direccion debe de ir de el otro objeto al bounding box;
		Vertex3 point = matrixOther.MultVertex(centerObjOther);
		int close = 0; double masCerca = DBL_MAX;
		for (int k = 0; k < normalesPlane.size(); k++) {
			double cerca = PlaneIntersection(point, direction, matrix.MultVector(normalesPlane[k]), matrix.MultVertex(puntosPlane[k])); //Quen tan comun son la normal contra mi vector de collision cos(0);
			if (abs(cerca) < abs(masCerca)) {
				close = k;
				masCerca = cerca;
			}
		}
		return matrix.MultVector(normalesPlane[close]);
	}

	//Plane Intersection
	static double PlaneIntersection(Vertex3 point, Vertex3 direction, Vertex3 normalPlane, Vertex3 pointPlane) {
		//PlaneEq = P*n – A*n = 0  (P and A points in plane)
		//Ray = P + dir * t
		//Substitus: ( P + dir * t ) * n = A * n
		//solve for t: T = ( A * n - P * n ) / ( dir * n)
		double t;
		double dirxn = direction & normalPlane;
		if (dirxn == 0) {
			t = DBL_MAX;
		} else {
			t = ((pointPlane & normalPlane) - (point & normalPlane)) / dirxn;
		}
		return t;
	}

private:
	double BBvals[6] = { 0,0,0,0,0,0 }; //X+-, Y+-, Z+-

	void CalculateBoundingBoxGameObject(GameObject* go) {
		bool first = false;
		for (int i = 0; i < go->meshes.size(); i++) {
			CalculateBoundingBoxMesh(go, i, first);
		}
		CreateBoundingBox();
	}
	void CalculateBoundingBoxMesh(GameObject* go, int mesh, bool& onlyMesh) {
		bool first; first = onlyMesh ? false : true;
		for (int j = 0; j < go->meshes[mesh].faces.size(); j++) {
			for (int k = 0; k < go->faces[go->meshes[mesh].faces[j]].count(); k++) {
				Vertex3 point = go->vertexes[go->faces[go->meshes[mesh].faces[j]].v[k]];
				if (!first) {
					BBvals[0] = point.GetX();
					BBvals[1] = point.GetX();
					BBvals[2] = point.GetY();
					BBvals[3] = point.GetY();
					BBvals[4] = point.GetZ();
					BBvals[5] = point.GetZ();
					first = true;
					onlyMesh = true;
				}
				if (point.GetX() > BBvals[0]) {
					BBvals[0] = point.GetX();
				}
				if (point.GetX() < BBvals[1]) {
					BBvals[1] = point.GetX();
				}
				if (point.GetY() > BBvals[2]) {
					BBvals[2] = point.GetY();
				}
				if (point.GetY() < BBvals[3]) {
					BBvals[3] = point.GetY();
				}
				if (point.GetZ() > BBvals[4]) {
					BBvals[4] = point.GetZ();
				}
				if (point.GetZ() < BBvals[5]) {
					BBvals[5] = point.GetZ();
				}
			}
		}
		CreateBoundingBox();
	}

	void CreateBoundingBox() {
		normals.clear();
		boundingBox.clear();
		centerBox.SetVertices((BBvals[0] + BBvals[1]) / 2, (BBvals[2] + BBvals[3]) / 2, (BBvals[4] + BBvals[5]) / 2);
		//ParaSAT
		normals.push_back(Vertex3(1, 0, 0));
		normals.push_back(Vertex3(0, 1, 0));
		normals.push_back(Vertex3(0, 0, 1));		
		boundingBox.push_back(Vertex3(BBvals[0], BBvals[2], BBvals[4]));
		boundingBox.push_back(Vertex3(BBvals[0], BBvals[2], BBvals[5]));
		boundingBox.push_back(Vertex3(BBvals[0], BBvals[3], BBvals[4]));
		boundingBox.push_back(Vertex3(BBvals[0], BBvals[3], BBvals[5]));
		boundingBox.push_back(Vertex3(BBvals[1], BBvals[2], BBvals[4]));
		boundingBox.push_back(Vertex3(BBvals[1], BBvals[2], BBvals[5]));
		boundingBox.push_back(Vertex3(BBvals[1], BBvals[3], BBvals[4]));
		boundingBox.push_back(Vertex3(BBvals[1], BBvals[3], BBvals[5]));
		//ParaCARA
		normalesPlane.push_back(Vertex3(1, 0, 0));
		normalesPlane.push_back(Vertex3(0, 1, 0));
		normalesPlane.push_back(Vertex3(0, 0, 1));
		normalesPlane.push_back(Vertex3(-1, 0, 0));
		normalesPlane.push_back(Vertex3(0, -1, 0));
		normalesPlane.push_back(Vertex3(0, 0, -1));
		puntosPlane.push_back(boundingBox[0]);
		puntosPlane.push_back(boundingBox[0]);
		puntosPlane.push_back(boundingBox[0]);
		puntosPlane.push_back(boundingBox[7]);
		puntosPlane.push_back(boundingBox[7]);
		puntosPlane.push_back(boundingBox[7]);
	}
};

//Hierarchcal //TODO
struct HierarchicalModel {
	//Hierarchical Model
	vector<vector<int>> hierarchical;

	void RemoveChildHierarchical(int hijo, int padre) {
		int hijosP = hierarchical[padre][0]; //Los hijos actuales de mi padre
		for (int i = 1; i <= hijosP; i++) {
			if (hierarchical[padre][i] == hijo) {
				hierarchical[padre][i] = hierarchical[padre][hijosP];
				hierarchical[padre].pop_back();
				i--; hijosP--;
				hierarchical[padre][0] = hijosP;
			}
		}
	}
	void AddChildHierarchical(int hijo, int padre) {
		int hijosP = hierarchical[padre][0]; //Los hijos actuales de mi padre
		hierarchical[padre].resize(hijosP + 2, 0); //Me agrego una casilla
		hierarchical[padre][0] = hijosP + 1; //Ub hijo mas
		hierarchical[padre][hijosP + 1] = hijo; //Soy yo
	}

};

//Camera
struct CameraTool {
private:
	Vertex2 mousePosAnt;
	bool Dim_3D_2D;
	//PanTiltMove Tool	
	Vertex2 rotCam;	float camZoom = 8; float MaxCamZomm = 8;
	bool panTool = false, zoomTool = false, moveTool = false;
	//Config Camera	
	float params3D[3]; //FoV-Y, Near, Far
	float params2D; //FoV-Y, Near, Far
public:
	Vertex3 CamaraCenter;
	Vertex3 CamaraEye; //3D y 3D
	bool ActivePanTilt = true;

	CameraTool() {} 
	void Init(float fovY, float nearZ, float farZ) { // 60, 0.1, 70
		Dim_3D_2D = true;
		params3D[0] = fovY;
		params3D[1] = nearZ;
		params3D[2] = farZ;
		rotCam.SetVertices(90, 90);
		CamaraCenter.SetVertices(0, 0, 0);
		CamaraEye.SetVertices(0, 0, camZoom);
	}
	void Init(float height) {
		Dim_3D_2D = false;
		params2D = height;
		rotCam.SetVertices(90, 90);
		CamaraCenter.SetVertices(0, 0, 0);
		CamaraEye.SetVertices(0, 0, camZoom);
	}
	
	void SetPoint(Vertex3 center, Vertex3 eye, Vertex2 rot) {
		CamaraCenter.SetVertices(center.GetX(), center.GetY(), center.GetZ());
		CamaraEye.SetVertices(eye.GetX(), eye.GetY(), eye.GetZ());
		rotCam.SetVertices(rot.GetX(), rot.GetY());
		camZoom = Vertex3::Distance(center, eye);
	}

	void processSpecialKeys(int key, int x, int y) {

	}
	void keyboard(unsigned char key, int x, int y) {

	}
	void mouse(int btn, int state, int x, int y) {
		if (state == GLUT_DOWN) {
			mousePosAnt.SetVertices(x, y);
			if (btn == GLUT_LEFT_BUTTON) {
				panTool = !panTool;
			}
			else if (btn == GLUT_RIGHT_BUTTON) {
				zoomTool = !zoomTool;
			}
			else if (btn == GLUT_MIDDLE_BUTTON) {
				moveTool = !moveTool;
			}
		}
	}
	void mouse_idle(int x, int y) {
		if (ActivePanTilt) {
			if (Dim_3D_2D) {
				PanTiltMove3D(Vertex2(x, y));
			} else {
				PanTiltMove2D(Vertex2(x, y));
			}
		}
		mousePosAnt.SetVertices(x, y);
	}

	//Set
	void Set() {
		if (Dim_3D_2D) {
			Set3D(0, 0, 1, 1);
		}else {
			Set2D(0, 0, 1, 1);
		}
	}
	void Set(int X, int Y, int divX, int divY) {
		if (Dim_3D_2D) {
			Set3D(X, Y, divX, divY);
		} else {
			Set2D(X, Y, divX, divY);
		}
	}

private:
	//PanTiltMove
	void PanTiltMove3D(Vertex2 mousePos) {
		if (panTool) {
			rotCam.SetVertices(rotCam.GetX() + (mousePos.GetX() - mousePosAnt.GetX()) / 3.0, rotCam.GetY() - (mousePos.GetY() - mousePosAnt.GetY()) / 3.0);
		}
		else if (zoomTool) {
			camZoom += (mousePosAnt.GetX() - mousePos.GetX()) / 10.0;
		}
		else if (moveTool) {
			TransformMatrix CamMat;
			CamMat.Identity();
			CamMat.Translated(CamaraCenter.GetX(), CamaraCenter.GetY(), CamaraCenter.GetZ());
			CamMat.RotateY(-rotCam.GetX() + 90);
			CamMat.RotateX(-rotCam.GetY());
			Vertex3 Point(((mousePosAnt.GetX() - mousePos.GetX()) / 100.0), 0, ((mousePosAnt.GetY() - mousePos.GetY()) / 100.0));
			CamaraCenter = CamMat.MultVertex(Point);
		}
		if (camZoom > MaxCamZomm) { camZoom = MaxCamZomm; }
		if (camZoom < 0) { camZoom = 0; }
		if (rotCam.GetX() > 360) { rotCam.SetX(360); }
		if (rotCam.GetX() < 0) { rotCam.SetX(0); }
		if (rotCam.GetY() > 360) { rotCam.SetY(360); }
		if (rotCam.GetY() < 0) { rotCam.SetY(0); }

		CamaraEye.SetX(CamaraCenter.GetX() + camZoom * sin(rotCam.GetY() * (M_PI / 180.0)) * cos(rotCam.GetX() * (M_PI / 180.0)));
		CamaraEye.SetZ(CamaraCenter.GetZ() + camZoom * sin(rotCam.GetY() * (M_PI / 180.0)) * sin(rotCam.GetX() * (M_PI / 180.0)));
		CamaraEye.SetY(CamaraCenter.GetY() + camZoom * cos(rotCam.GetY() * (M_PI / 180.0)));
	}
	void PanTiltMove2D(Vertex2 mousePos) {
		if (zoomTool) {
			params2D += (mousePosAnt.GetX() - mousePos.GetX()) / 10.0;
		}
		else if (moveTool) {
			TransformMatrix CamMat;
			CamMat.Identity();
			CamMat.Translated(CamaraEye.GetX(), CamaraEye.GetY(), CamaraEye.GetZ());
			Vertex3 Point(((mousePosAnt.GetX() - mousePos.GetX()) / 100.0), -((mousePosAnt.GetY() - mousePos.GetY()) / 100.0), 0);
			CamaraEye = CamMat.MultVertex(Point);
		}
		if (camZoom > MaxCamZomm) { camZoom = MaxCamZomm; }
		if (camZoom < 0) { camZoom = 0; }

	}

	//Set
	void Set3D(int X, int Y, int divX, int divY) {
		float w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
		int sizeX = (w / divX), sizeY = (h / divY);
		int iniX = X * sizeX, iniY = Y * sizeY;
		glViewport(iniX, iniY, (GLsizei)sizeX, (GLsizei)sizeY); //set the viewport size, based on fuction input 
		glMatrixMode(GL_PROJECTION); glLoadIdentity(); //set the projection matrix set as identity	
		//3D
		gluPerspective(params3D[0], (GLfloat)sizeX / (GLfloat)sizeY, params3D[1], params3D[2]); //then set perspective projection parameters based on aspect ratio
		glMatrixMode(GL_MODELVIEW); glLoadIdentity(); //set the model view matrix to identity
		gluLookAt(CamaraEye.GetX(), CamaraEye.GetY(), CamaraEye.GetZ(), CamaraCenter.GetX(), CamaraCenter.GetY(), CamaraCenter.GetZ(), 0.0, 1.0, 0.0);
		Light::SetAll(); //Para que no se peguen a mi camara
	}
	void Set2D(int X, int Y, int divX, int divY) {
		float w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
		int sizeX = (w / divX), sizeY = (h / divY);
		int iniX = X * sizeX, iniY = Y * sizeY;
		glViewport(iniX, iniY, (GLsizei)sizeX, (GLsizei)sizeY); //set the viewport size, based on fuction input 
		glMatrixMode(GL_PROJECTION); glLoadIdentity(); //set the projection matrix set as identity	
		//2D 
		float ww = params2D * ((GLfloat)sizeX / (GLfloat)sizeY);
		gluOrtho2D(CamaraEye.GetX() - (ww / 2.0), CamaraEye.GetX() + (ww / 2.0), CamaraEye.GetY() - (params2D / 2.0), CamaraEye.GetY() + (params2D / 2.0));
		Light::SetAll(); //Para que no se peguen a mi camara
	}
};

//User Interface //TODO
struct UserInterface {

};

// Autonomus //TODO
struct AutonomusMovement {
	Vertex3* position;
	Vertex3* rotation;
	vector<Vertex3*> path;
	int indexPath;
	double recorrido;
	double velRecorriddo;
	double velocidad;
	bool enablePath;

	void AddPath(Vertex3* p) {
		path.push_back(p);
	}

	void CalculoPathSig(int index) {
		indexPath = index;
		recorrido = 0;
		position = path[indexPath];
		Vertex3 diff = *path[(indexPath + 1) % path.size()] - *path[indexPath];
		velRecorriddo = velocidad / diff.Magnitud();
		double teta = atan2(diff.GetX(), diff.GetZ()) * (180.0 / M_PI);
		double phi = atan2(sqrt(pow(diff.GetZ(), 2) + pow(diff.GetX(), 2)), diff.GetY()) * (180.0 / M_PI);
		rotation->SetVertices(phi - 90, teta, 0);
	}
	void DisaplePathAutom() { enablePath = false; }
	void EnablePathAutom(double vel) {
		enablePath = true;
		velocidad = vel;
		CalculoPathSig(0);
	}

	void Draw() {
		if (enablePath) {
			*position = *path[indexPath] + (*path[(indexPath + 1) % path.size()] - *path[indexPath]) * recorrido;
			recorrido += velRecorriddo;
			if (recorrido >= 1) {
				CalculoPathSig((indexPath + 1) % path.size());
			}
		}
	}

};

//Autonomus Movement //TODO
struct AutonomusMachine {
	float aTorso, aHead, aShoulderR, aElbowR, aWristR, aShoulderL, aElbowL, aWristL;
	float *position; 

	int actBaile, posCount; float trans;
	float ***bailes; int *cantPos;
	bool enabled;

	AutonomusMachine() {
		position = new float[3]{ 0,0,0 };		
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

	void Draw() {
		if (enabled) {
			UpdateParamsGameObject(bailes[actBaile - 1], posCount, cantPos[actBaile - 1], trans);
			//DRAW GameObject
			trans += 0.05;
			if (trans > 1) { trans = 0; posCount++; }
			if (posCount >= cantPos[actBaile - 1]) { posCount = 0; }
		}
	}

	void UpdateParamsGameObject(float **posicion, int sigPos, int totPos, float trans) { //trans va de 0 a 1
		aTorso = posicion[(sigPos - 1 + totPos) % totPos][0] + trans*(posicion[sigPos][0] - posicion[(sigPos - 1 + totPos) % totPos][0]);
		aHead = posicion[(sigPos - 1 + totPos) % totPos][1] + trans*(posicion[sigPos][1] - posicion[(sigPos - 1 + totPos) % totPos][1]);
		aShoulderR = posicion[(sigPos - 1 + totPos) % totPos][2] + trans*(posicion[sigPos][2] - posicion[(sigPos - 1 + totPos) % totPos][2]);
		aElbowR = posicion[(sigPos - 1 + totPos) % totPos][3] + trans*(posicion[sigPos][3] - posicion[(sigPos - 1 + totPos) % totPos][3]);
		aWristR = posicion[(sigPos - 1 + totPos) % totPos][4] + trans*(posicion[sigPos][4] - posicion[(sigPos - 1 + totPos) % totPos][4]);
		aShoulderL = posicion[(sigPos - 1 + totPos) % totPos][5] + trans*(posicion[sigPos][5] - posicion[(sigPos - 1 + totPos) % totPos][5]);
		aElbowL = posicion[(sigPos - 1 + totPos) % totPos][6] + trans*(posicion[sigPos][6] - posicion[(sigPos - 1 + totPos) % totPos][6]);
		aWristL = posicion[(sigPos - 1 + totPos) % totPos][7] + trans*(posicion[sigPos][7] - posicion[(sigPos - 1 + totPos) % totPos][7]);
	}

};

//Normals
//TODO Normal for each Vertex, more smooth
struct NormalsTool {
public:
	static void CalculateNormalsFace(GameObject* go) {
		go->vertexNormals.clear();
		for (int f = 0; f < go->faces.size(); f++) { //Calculo la normal de la cara
			Vertex3 normal = CalculateNormal(go->vertexes[go->faces[f].v[0]], go->vertexes[go->faces[f].v[1]], go->vertexes[go->faces[f].v[2]]);
			go->vertexNormals.push_back(normal);
			go->faces[f].vn.clear();
			for (int j = 0; j < go->faces[f].count(); j++) { //La asigno a los vertces de la cara
				go->faces[f].vn.push_back(f);
			}
		}
	}

	/*static void CalculateNormalsVertex(GameObject* go) { //Debo buscar todas las caras que utilicen el mismo vetexes
	int cantCarasInVert;
	vector<Vertex3> newVetexNormals; //Una normal por vector
	for (int v = 0; v < go->vertexes.size(); v++) newVetexNormals.push_back(*(new Vertex3()));
	for (int v = 0; v < go->vertexes.size(); v++) { //Analizare este vertex
	cantCarasInVert = 0;
	for (int f = 0; f < go->faces.size(); f++) { //Estara en esta cara
	for (int i = 0; i < go->faces[f].count(); i++) { //chequemos
	if (v == go->faces[f].v[i]) {
	newVetexNormals[v] += go->vertexNormals[go->faces[f].vn[i]];
	cantCarasInVert++;
	faces[f].vn[i] = v + 1;
	}
	}
	}
	newVetexNormals[v] /= cantCarasInVert;
	}
	go->vertexNormals.clear();
	for (int i = 0; i < newVetexNormals.size(); i++) {
	go->vertexNormals.push_back(newVetexNormals[i]);
	}
	}*/

	static void CalculateNormalOneFace(GameObject* go, int face) {
		Vertex3 normal = CalculateNormal(go->vertexes[go->faces[face].v[0]], go->vertexes[go->faces[face].v[1]], go->vertexes[go->faces[face].v[2]]);
		go->vertexNormals.push_back(normal);
		go->faces[face].vn.clear();
		for (int j = 0; j < go->faces[face].count(); j++) { //La asigno a los vertces de la cara
			go->faces[face].vn.push_back(go->vertexNormals.size() - 1);
		}
	}
	static Vertex3 CalculateNormal(Vertex3 v1, Vertex3 v2, Vertex3 v3) {
		Vertex3 a = v1 - v2;
		Vertex3 b = v1 - v3;
		Vertex3 c = a*b;
		c.Unitario();
		return c;
	}
};

//OBJ
struct ReadMaterial {
public:
	static void readMaterial(Material& material, bool& mat, Texture& textura, bool& tex, string filename, string name) {
		ifstream file(filename);
		string caso;
		bool thisOne = false;
		if (!file) return;
		while (file >> caso) {
			if (caso == "newmtl") {
				string aux; file >> aux;
				thisOne = (aux == name);
				if (aux == name) { mat = true; }
			}
			else if ((caso == "Kd") && thisOne) {
				double r, g, b;
				file >> r; file >> g; file >> b;
				material.Diffuse(r, g, b);
			}
			else if ((caso == "Ka") && thisOne) {
				double r, g, b;
				file >> r; file >> g; file >> b;
				material.Ambient(r, g, b);
			}
			else if ((caso == "Ks") && thisOne) {
				double r, g, b;
				file >> r; file >> g; file >> b;
				material.Specular(r, g, b);
			}
			else if ((caso == "Ns") && thisOne) {
				double s;
				file >> s;
				material.Shinnes(s);
			}
			else if ((caso == "map_Kd") && thisOne) {
				string aux; file >> aux;
				ReadBitMap::ReadBMPFile(&textura, aux);
				tex = true;
			}
		}
	}
};

struct ReadModelOBJ {
public:
	static void readOBJ(GameObject* go, int mesh, string filename) {
		ifstream file(filename);
		string mtllib;
		string caso;
		if (!file) return;
		while (file >> caso) {
			if (caso == "v") {
				Vertex3 vec; double aux;
				file >> aux; vec.SetX(aux);
				file >> aux; vec.SetY(aux);
				file >> aux; vec.SetZ(aux); 
				go->vertexes.push_back(vec);
			}
			else if (caso == "vn") {
				Vertex3 vec; double aux;
				file >> aux; vec.SetX(aux);
				file >> aux; vec.SetY(aux);
				file >> aux; vec.SetZ(aux);
				go->vertexNormals.push_back(vec);
			}
			else if (caso == "vt") {
				Vertex2 vec; double aux;
				file >> aux; vec.SetX(aux);
				file >> aux; vec.SetY(aux);
				go->vertexTexture.push_back(vec);
			}
			else if (caso == "f") {
				Face face;
				string linea, fv, dato; int k, i = 0;
				getline(file, linea, '\n');
				istringstream SSlinea(linea);
				while (SSlinea >> fv) {
					istringstream verticesF(fv); k = 0;
					while (getline(verticesF, dato, '/')) {
						if (k == 0) { if (dato != "") { face.v.push_back(stoi(dato)-1); } else { face.v.push_back(-1); } } //Vertice
						if (k == 1) { if (dato != "") { face.vt.push_back(stoi(dato)-1); } else { face.vt.push_back(-1); } } //Texture
						if (k == 2) { if (dato != "") { face.vn.push_back(stoi(dato)-1); } else { face.vn.push_back(-1); } } //Normal
						k++;
					}
					i++;
				}
				go->pushFace(face, mesh);
			}
			else if (caso == "mtllib") {
				string aux; file >> aux;
				mtllib = aux;
			}
			else if (caso == "usemtl") {
				string aux; file >> aux;	
				bool mat = false, tex = false; int m = go->material.size(), t = go->textura.size();
				go->material.resize(m + 1);
				go->textura.resize(t + 1);						
				ReadMaterial::readMaterial(go->material[m], mat, go->textura[t], tex, mtllib, aux);
				if(!mat) go->material.resize(m);
				if(!tex) go->textura.resize(t);
				mesh = go->pushMesh(mat ? m : -1, tex ? t : -1);
			}
		}

		NormalsTool::CalculateNormalsFace(go);
	}
};

//MyShapes
//TODO option to make texture
struct BasicShapesTool {
public:
	static void MakeShape(GameObject* go, int mesh, Shape shp, double width, double height, double depth, int resolution) {
		if (shp == Shape::Cube) {
			MakeCube(go, mesh);
			TransformMatrix mat; mat.Identity(); mat.Scale(width, height, depth);
			mat.MultGameObjectMesh(go, mesh);
			NormalsTool::CalculateNormalsFace(go);
		}
		if (shp == Shape::Icosaedro) {
			MakeIcosaedro(go, mesh);
			TransformMatrix mat; mat.Identity(); mat.Scale(width, height, depth);
			mat.MultGameObjectMesh(go, mesh);
			NormalsTool::CalculateNormalsFace(go);
		}
		if (shp == Shape::Sphere) {
			MakeSphere(go, mesh, resolution);
			TransformMatrix mat; mat.Identity(); mat.Scale(width, height, depth);
			mat.MultGameObjectMesh(go, mesh);
			NormalsTool::CalculateNormalsFace(go);
		}
		if (shp == Shape::Quad) {
			MakeQuad(go, mesh);
			TransformMatrix mat; mat.Identity(); mat.Scale(width, height, depth);
			mat.MultGameObjectMesh(go, mesh);
			NormalsTool::CalculateNormalsFace(go);
		}
	}

private:
	static void MakeQuad(GameObject* go, int mesh) {
		int os = go->vertexes.size(); //offset
		int ost = go->vertexTexture.size(); //offset Textura

		go->setVertexes(new Vertex3[4]{ *(new Vertex3(-0.5, 0, -0.5)), *(new Vertex3(-0.5, 0, 0.5)),
			*(new Vertex3(0.5, 0, 0.5)), *(new Vertex3(0.5, 0, -0.5)) }, 4);
		go->setVertexesTexture(new Vertex2[4]{ *(new Vertex2(0,0)), *(new Vertex2(1,0)) ,
			*(new Vertex2(1,1)) , *(new Vertex2(0,1)) }, 4);
		Face *f; f = new Face(); 
		f->setV(new int[4]{ os + 0,os + 1,os + 2,os + 3 }, 4);
		f->setVT(new int[4]{ ost + 0, ost + 1, ost + 2, ost + 3 }, 4);
		go->pushFace(*f, mesh);
	}
	static void MakeCube(GameObject* go, int mesh) {
		int os = go->vertexes.size(); //offset
		int ost = go->vertexTexture.size(); //offset Textura

		go->setVertexes(new Vertex3[8]{ *(new Vertex3(-0.5, -0.5, -0.5)), *(new Vertex3(-0.5, -0.5, 0.5)),
			*(new Vertex3(-0.5, 0.5, 0.5)), *(new Vertex3(-0.5, 0.5, -0.5)),
			*(new Vertex3(0.5, 0.5, -0.5)), *(new Vertex3(0.5, 0.5, 0.5)),
			*(new Vertex3(0.5, -0.5, 0.5)), *(new Vertex3(0.5, -0.5, -0.5)) }, 8);
		Face *f;
		f = new Face(); f->setV(new int[4]{ os + 0,os + 1,os + 2,os + 3 }, 4); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[4]{ os + 4,os + 5,os + 6,os + 7 }, 4); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[4]{ os + 1,os + 0,os + 7,os + 6 }, 4); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[4]{ os + 6,os + 5,os + 2,os + 1 }, 4); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[4]{ os + 0,os + 3,os + 4,os + 7 }, 4); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[4]{ os + 5,os + 4,os + 3,os + 2 }, 4); go->pushFace(*f, mesh);
	}
	static void MakeSphere(GameObject* go, int mesh, int res) {
		int os = go->vertexes.size(); //offset
		int ost = go->vertexTexture.size(); //offset Textura

		for (int i = 0; i <= res; i++) {
			double phi = i * 180.0 / res;
			for (int j = 0; j <= res; j++) {
				double teta = j * 360.0 / res;
				Vertex3* vec = new Vertex3();
				vec->SetX(sin(phi * (M_PI / 180.0)) * cos(teta * (M_PI / 180.0)));
				vec->SetZ(sin(phi * (M_PI / 180.0)) * sin(teta * (M_PI / 180.0)));
				vec->SetY(cos(phi * (M_PI / 180.0)));
				Vertex2* vecTex = new Vertex2((double)j / (double)res, (double)i / (double)res);
				go->vertexTexture.push_back(*vecTex);
				go->vertexes.push_back(*vec);
			}
		}

		res++;
		//Los triangulos de arriba
		for (int j = 0; j < res; j++) { //1 menos por el ultimo			
			Face *f; f = new Face(); int i = 0;
			f->setV(new int[3]{ os + ((i + 0)*res + (j + 1)), os + ((i + 1)*res + (j + 1)), os + ((i + 1)*res + (j + 0)) }, 3);
			f->setVT(new int[3]{ ost + ((i + 0)*res + (j + 1)), ost + ((i + 1)*res + (j + 1)), ost + ((i + 1)*res + (j + 0)) }, 3);
			go->pushFace(*f, mesh);
		}
		//Los triangulos de abajo
		for (int j = 0; j < res; j++) { //1 menos por el ultimo			
			Face *f; f = new Face(); int i = res - 2;
			f->setV(new int[3]{ os + ((i + 0)*res + (j + 1)), os + ((i + 1)*res + (j + 0)), os + ((i + 0)*res + (j + 0)) }, 3);
			f->setVT(new int[3]{ ost + ((i + 0)*res + (j + 1)), ost + ((i + 1)*res + (j + 0)), ost + ((i + 0)*res + (j + 0)) }, 3);
			go->pushFace(*f, mesh);
		}
		//La circunferencia
		for (int i = 1; i < res - 2; i++) { //2 menos por los de hasta arriba
			for (int j = 0; j < res; j++) { //1 menos por el ultimo
				Face *f; f = new Face();
				f->setV(new int[4]{ os + ((i + 0)*res + (j + 1)), os + ((i + 1)*res + (j + 1)), os + ((i + 1)*res + (j + 0)), os + ((i + 0)*res + (j + 0)) }, 4);
				f->setVT(new int[4]{ ost + ((i + 0)*res + (j + 1)), ost + ((i + 1)*res + (j + 1)), ost + ((i + 1)*res + (j + 0)), ost + ((i + 0)*res + (j + 0)) }, 4);
				go->pushFace(*f, mesh);
			}
		}
	}
	static void MakeIcosaedro(GameObject* go, int mesh) {
		int os = go->vertexes.size(); //offset
		int ost = go->vertexTexture.size(); //offset Textura

		float a = 0.525731112119133606;
		float b = 0.850650808352039932;
		go->setVertexes(new Vertex3[12]{
			*(new Vertex3(-a, 0.0, b)),*(new Vertex3(a, 0.0, b)), *(new Vertex3(-a, 0.0, -b)),*(new Vertex3(a, 0.0, -b)),
			*(new Vertex3(0.0, b, a)), *(new Vertex3(0.0, b, -a)),*(new Vertex3(0.0, -b, a)), *(new Vertex3(0.0, -b, -a)),
			*(new Vertex3(b, a, 0.0)), *(new Vertex3(-b, a, 0.0)),*(new Vertex3(b, -a, 0.0)), *(new Vertex3(-b, -a, 0.0)) }, 12);
		Face *f;
		f = new Face(); f->setV(new int[3]{ os + 1,os + 4,os + 0 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 4,os + 9,os + 0 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 4,os + 5,os + 9 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 8,os + 5,os + 4 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 1,os + 8,os + 4 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 1,os + 10,os + 8 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 10,os + 3,os + 8 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 8,os + 3,os + 5 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 3,os + 2,os + 5 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 3,os + 7,os + 2 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 3,os + 10,os + 7 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 10,os + 6,os + 7 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 6,os + 11,os + 7 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 6,os + 0,os + 11 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 6,os + 1,os + 0 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 10,os + 1,os + 6 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 11,os + 0,os + 9 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 2,os + 11,os + 9 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 5,os + 2,os + 9 }, 3); go->pushFace(*f, mesh);
		f = new Face(); f->setV(new int[3]{ os + 11,os + 2,os + 7 }, 3); go->pushFace(*f, mesh);
	}
};

//Revolusion
//TODO Option to make texture
struct SolidRevolutionTool {
public: 
	static void MakeSolid(GameObject* go, int mesh, Vertex3 eje, Vertex3 point, int* vertProfile, int nVert, int resolution, bool doble) {
		TransformMatrix mat;
		mat.Identity();
		double teta = atan2(eje.GetX(), eje.GetZ()) * (180.0 / M_PI);
		double phi = atan2(sqrt(pow(eje.GetZ(), 2) + pow(eje.GetX(), 2)), eje.GetY()) * (180.0 / M_PI);
		mat.RotateY(teta);
		mat.RotateX(phi);
		mat.Translated(point.GetX(), point.GetY(), point.GetZ());
		//MakeSolid
		double delta = 360.0 / resolution;
		int cantVert = go->vertexes.size();
		//Genero Puntos
		for (int i = 1; i <= resolution; i++) {
			mat.RotateY(delta);
			for (int j = 0; j < nVert; j++) {
				Vertex3* vec = new Vertex3();
				*vec = mat.MultVertex(go->vertexes[vertProfile[j]]);
				go->vertexes.push_back(*vec);
			}
		}
		//Pongo Caras
		for (int i = 0; i < resolution; i++) {
			for (int j = 0; j < cantVert-1; j++) {
				Face *f; 
				f = new Face();
				f->setV(new int[4]{ ((i + 0)*cantVert + (j + 0)), ((i + 1)*cantVert + (j + 0)), ((i + 1)*cantVert + (j + 1)), ((i + 0)*cantVert + (j + 1)) }, 4);
				go->pushFace(*f, mesh);
				if (doble) {
					f = new Face();
					f->setV(new int[4]{ ((i + 0)*cantVert + (j + 1)), ((i + 1)*cantVert + (j + 1)), ((i + 1)*cantVert + (j + 0)), ((i + 0)*cantVert + (j + 0)) }, 4);
					go->pushFace(*f, mesh);
				}
			}
		}

		NormalsTool::CalculateNormalsFace(go);
	}

};

//Extrusion
struct ExtrusionTool {
public:
	static void Extrude(GameObject* go, int face, int mesh, Vertex3 translate, Vertex3 direction) {
		TransformMatrix matrix;
		matrix.Identity();
		Vertex3 normal = go->vertexNormals[go->faces[face].vn[0]];
		int cantVert = go->faces[face].v.size();
		int totVert = go->vertexes.size();
		double teta, phi;
		Face * f;
		//CenterFace
		Vertex3 centerFace;
		for (int i = 0; i < cantVert; i++) {
			centerFace = centerFace + go->vertexes[go->faces[face].v[i]];
		}
		centerFace = centerFace / cantVert;
		//Translate Direction
		matrix.Translated(translate.GetX(), translate.GetY(), translate.GetZ());
		//Translate Center Face
		matrix.Translated(centerFace.GetX(), centerFace.GetY(), centerFace.GetZ());
		//Roto en Direccion
		teta = atan2(direction.GetX(), direction.GetZ()) * (180.0 / M_PI);
		phi = atan2(sqrt(pow(direction.GetZ(), 2) + pow(direction.GetX(), 2)), direction.GetY()) * (180.0 / M_PI);
		matrix.RotateY(+teta);
		matrix.RotateX(-phi);
		//Rotate to Y
		teta = atan2(normal.GetX(), normal.GetZ()) * (180.0 / M_PI);
		phi = atan2(sqrt(pow(normal.GetZ(), 2) + pow(normal.GetX(), 2)), normal.GetY()) * (180.0 / M_PI);
		matrix.RotateX(phi);
		matrix.RotateY(-teta);
		//Translate Center
		matrix.Translated(-centerFace.GetX(), -centerFace.GetY(), -centerFace.GetZ());
		//Make New Vertices
		for (int i = 0; i < cantVert; i++) {
			go->vertexes.push_back(matrix.MultVertex(go->vertexes[go->faces[face].v[i]]));
		}
		//Make contour Face
		for (int i = 0; i < cantVert - 1; i++) {
			f = new Face();
			f->v.push_back(go->faces[face].v[i]);
			f->v.push_back(go->faces[face].v[i + 1]);
			f->v.push_back(totVert + i + 1);
			f->v.push_back(totVert + i);
			if (go->meshes[mesh].textura>=0) {
				f->vt.push_back(go->faces[face].vt[i]);
				f->vt.push_back(go->faces[face].vt[i + 1]);
				f->vt.push_back(go->faces[face].vt[i + 1]);
				f->vt.push_back(go->faces[face].vt[i]);
			}
			go->pushFace(*f, mesh);
			NormalsTool::CalculateNormalOneFace(go, go->faces.size() - 1);
		}
		//La Ultima
		f = new Face();
		f->v.push_back(go->faces[face].v[cantVert - 1]);
		f->v.push_back(go->faces[face].v[0]);
		f->v.push_back(totVert);
		f->v.push_back(totVert + cantVert - 1);
		if (go->meshes[mesh].textura >= 0) {
			f->vt.push_back(go->faces[face].vt[cantVert - 1]);
			f->vt.push_back(go->faces[face].vt[0]);
			f->vt.push_back(go->faces[face].vt[0]);
			f->vt.push_back(go->faces[face].vt[cantVert - 1]);
		}
		go->pushFace(*f, mesh);
		NormalsTool::CalculateNormalOneFace(go, go->faces.size() - 1);

		//Make Top Face
		f = new Face();
		for (int i = 0; i < cantVert; i++) {
			if (go->meshes[mesh].textura >= 0) {
				f->vt.push_back(go->faces[face].vt[i]);
			}
			f->v.push_back(totVert + i);
		}
		go->pushFace(*f, mesh);
		NormalsTool::CalculateNormalOneFace(go, go->faces.size() - 1);

	}
	static void ExtrudeProfile(GameObject* go, int mesh, int* vertProfile, int* vertNormProfile, int* vertTextureProfile, int cantVert, Vertex3 translate, Vertex3 direction, Vertex3 scale) {
		TransformMatrix matrix;
		matrix.Identity();
		int totVert = go->vertexes.size();
		int totVertText = go->vertexTexture.size();
		double teta, phi;
		Face * f;
		//CenterProfile
		Vertex3 centerProfile;
		for (int i = 0; i < cantVert; i++) {
			centerProfile = centerProfile + go->vertexes[vertProfile[i]];
		}
		centerProfile = centerProfile / cantVert;
		//Translate Direction
		matrix.Translated(translate.GetX(), translate.GetY(), translate.GetZ());
		//Translate Center Face
		matrix.Translated(centerProfile.GetX(), centerProfile.GetY(), centerProfile.GetZ());
		//Roto en Direccion
		teta = atan2(direction.GetX(), direction.GetZ()) * (180.0 / M_PI);
		phi = atan2(sqrt(pow(direction.GetZ(), 2) + pow(direction.GetX(), 2)), direction.GetY()) * (180.0 / M_PI);
		matrix.RotateY(+teta);
		matrix.RotateX(-phi);
		//Scale
		matrix.Scale(scale.GetX(), scale.GetY(), scale.GetZ());
		//Translate Center
		matrix.Translated(-centerProfile.GetX(), -centerProfile.GetY(), -centerProfile.GetZ());
		//Make New Vertices
		for (int i = 0; i < cantVert; i++) {
			go->vertexes.push_back(matrix.MultVertex(go->vertexes[vertProfile[i]]));
			if (go->meshes[mesh].textura >= 0) go->vertexTexture.push_back(*(new Vertex2(go->vertexTexture[vertTextureProfile[i]].GetX(), go->vertexTexture[vertTextureProfile[i]].GetY())));
		}
		//Make contour Face
		for (int i = 0; i < cantVert - 1; i++) {
			Vertex3 normal = NormalsTool::CalculateNormal(go->vertexes[vertProfile[i]], go->vertexes[vertProfile[i + 1]], go->vertexes[totVert + i]);
			f = new Face();
			if ((normal - go->vertexNormals[vertNormProfile[i]]).Magnitud() < normal.Magnitud()) { //Se supone es 0 y apuntan al mismo lado
				f->v.push_back(vertProfile[i]);
				f->v.push_back(vertProfile[i + 1]);
				f->v.push_back(totVert + i + 1);
				f->v.push_back(totVert + i);
				if (go->meshes[mesh].textura >= 0) {
					f->vt.push_back(vertTextureProfile[i]);
					f->vt.push_back(vertTextureProfile[i + 1]);
					f->vt.push_back(totVertText + i + 1);
					f->vt.push_back(totVertText + i);
				}
			}
			else {
				f->v.push_back(totVert + i);
				f->v.push_back(totVert + i + 1);
				f->v.push_back(vertProfile[i + 1]);
				f->v.push_back(vertProfile[i]);
				if (go->meshes[mesh].textura >= 0) {
					f->vt.push_back(totVertText + i);
					f->vt.push_back(totVertText + i + 1);
					f->vt.push_back(vertTextureProfile[i + 1]);
					f->vt.push_back(vertTextureProfile[i]);
				}
			}
			go->pushFace(*f, mesh);
		}

	}

};

//Subdivide
struct SubdivideTool {
public:
	static void Subdivide(GameObject* go, int mesh, int resolution) {
		for (int i = 0; i < go->faces.size(); i++) {
			go->faces[i].visible = false;
			if (go->faces[i].v.size() == 3) {
				if (go->meshes[mesh].textura>=0)
					SubdivideTriangulo(go, mesh, true, go->vertexes[go->faces[i].v[0]], go->vertexes[go->faces[i].v[1]], go->vertexes[go->faces[i].v[2]], go->vertexTexture[go->faces[i].vt[0]], go->vertexTexture[go->faces[i].vt[1]], go->vertexTexture[go->faces[i].vt[2]], resolution);
				else
					SubdivideTriangulo(go, mesh, false, go->vertexes[go->faces[i].v[0]], go->vertexes[go->faces[i].v[1]], go->vertexes[go->faces[i].v[2]], Vertex2::zero, Vertex2::zero, Vertex2::zero, resolution);
			}
			else if (go->faces[i].v.size() == 4) {
				if (go->meshes[mesh].textura >= 0)
					SubdivideCuadrado(go, mesh, true, go->vertexes[go->faces[i].v[0]], go->vertexes[go->faces[i].v[1]], go->vertexes[go->faces[i].v[2]], go->vertexes[go->faces[i].v[3]], go->vertexTexture[go->faces[i].vt[0]], go->vertexTexture[go->faces[i].vt[1]], go->vertexTexture[go->faces[i].vt[2]], go->vertexTexture[go->faces[i].vt[3]], resolution);
				else
					SubdivideCuadrado(go, mesh, false, go->vertexes[go->faces[i].v[0]], go->vertexes[go->faces[i].v[1]], go->vertexes[go->faces[i].v[2]], go->vertexes[go->faces[i].v[3]], Vertex2::zero, Vertex2::zero, Vertex2::zero, Vertex2::zero, resolution);
			}
		}
	}

private:
	static void SubdivideTriangulo(GameObject* go, int mesh, bool text, Vertex3 v1, Vertex3 v2, Vertex3 v3, Vertex2 vt1, Vertex2 vt2, Vertex2 vt3, int depth) {
		if (depth < 0) return;
		if (depth == 0) {
			int indexV = go->vertexes.size();
			int indexVT = go->vertexTexture.size();
			Face *f; f = new Face();
			if (text) {
				go->vertexTexture.push_back(vt1);
				go->vertexTexture.push_back(vt2);
				go->vertexTexture.push_back(vt3);
				f->setVT(new int[3]{ indexVT + 0,indexVT + 1,indexVT + 2 }, 3);
			}
			go->vertexes.push_back(v1);
			go->vertexes.push_back(v2);
			go->vertexes.push_back(v3);
			f->setV(new int[3]{ indexV + 0,indexV + 1,indexV + 2 }, 3); 

			go->pushFace(*f, mesh);
			return;
		}
		Vertex2 st12, st23, st31;
		st12 = vt1 + ((vt2 - vt1) * 0.5);
		st23 = vt2 + ((vt3 - vt2) * 0.5);
		st31 = vt3 + ((vt1 - vt3) * 0.5);
		Vertex3 s12, s23, s31;
		s12 = v1 + ((v2 - v1) * 0.5);
		s23 = v2 + ((v3 - v2) * 0.5);
		s31 = v3 + ((v1 - v3) * 0.5);

		SubdivideTriangulo(go, mesh, text, v1, s12, s31, vt1, st12, st31, depth - 1); //Llamo
		SubdivideTriangulo(go, mesh, text, s12, v2, s23, st12, vt2, st23, depth - 1);
		SubdivideTriangulo(go, mesh, text, s31, s23, v3, st31, st23, vt3, depth - 1);
		SubdivideTriangulo(go, mesh, text, s12, s23, s31, st12, st23, st31, depth - 1);
	}
	static void SubdivideCuadrado(GameObject* go, int mesh, bool text, Vertex3 v1, Vertex3 v2, Vertex3 v3, Vertex3 v4, Vertex2 vt1, Vertex2 vt2, Vertex2 vt3, Vertex2 vt4, int depth) {
		if (depth < 0) return;
		if (depth == 0) {
			int indexV = go->vertexes.size();
			int indexVT = go->vertexTexture.size();
			Face *f; f = new Face();
			if (text) {
				go->vertexTexture.push_back(vt1);
				go->vertexTexture.push_back(vt2);
				go->vertexTexture.push_back(vt3);
				go->vertexTexture.push_back(vt4);
				f->setVT(new int[4]{ indexVT + 0,indexVT + 1,indexVT + 2,indexVT + 3 }, 4);
			}
			go->vertexes.push_back(v1);
			go->vertexes.push_back(v2);
			go->vertexes.push_back(v3);
			go->vertexes.push_back(v4);
			f->setV(new int[4]{ indexV + 0,indexV + 1,indexV + 2,indexV + 3 }, 4); 

			go->pushFace(*f, mesh);
			return;
		}
		Vertex2 st12, st23, st34, st41, stC;
		st12 = vt1 + ((vt2 - vt1) * 0.5);
		st23 = vt2 + ((vt3 - vt2) * 0.5);
		st34 = vt3 + ((vt4 - vt3) * 0.5);
		st41 = vt4 + ((vt1 - vt4) * 0.5);
		stC = st12 + ((st34 - st12) * 0.5);
		Vertex3 s12, s23, s34, s41, sC;
		s12 = v1 + ((v2 - v1) * 0.5);
		s23 = v2 + ((v3 - v2) * 0.5);
		s34 = v3 + ((v4 - v3) * 0.5);
		s41 = v4 + ((v1 - v4) * 0.5);
		sC = s12 + ((s34 - s12) * 0.5);

		SubdivideCuadrado(go, mesh, text, v1, s12, sC, s41, vt1, st12, stC, st41, depth - 1); //Llamo
		SubdivideCuadrado(go, mesh, text, s12, v2, s23, sC, st12, vt2, st23, stC, depth - 1);
		SubdivideCuadrado(go, mesh, text, sC, s23, v3, s34, stC, st23, vt3, st34, depth - 1);
		SubdivideCuadrado(go, mesh, text, s41, sC, s34, v4, st41, stC, st34, vt4, depth - 1);
	}

};

