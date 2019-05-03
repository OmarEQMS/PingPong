#pragma once

struct ParametricSphere {
	Material material;
	Vertex3 center;
	double radius;

	ParametricSphere(Vertex3 c, double r) {
		center = c;
		radius = r;
	}
	double IntersectionWithRay(Vertex3 P0, Vertex3 direction) { //return closest intersection
		/*
		& = dot Product
		Ray = P0 + dir * t
		Sphere = ( (P - C) & (P - C) ) - r^2 = 0
		Substitus and Simplify
		t^2 (dir & dir) + 2t dir & (P0-C) + (P0-C) & (P0-C) - r^2 = 0
		t = ( -b +- sqrt(b^2 - 4*a*c) ) / 2*a
		*/
		double a, b, c, discrim, r1, r2, least;
		a = direction & direction;
		b = 2 * (direction & (P0 - center));
		c = ((P0 - center) & (P0 - center)) - pow(radius, 2);
		discrim = pow(b, 2) - 4 * a * c;
		if (discrim < 0) {
			return -1;
		}
		else {
			r1 = (-b + sqrt(discrim)) / 2 * a;
			r2 = (-b - sqrt(discrim)) / 2 * a;
			if (r1 < r2 && 0 < r1) least = r1;
			else if (0 < r2) least = r2;
			else least = -1;

			return least;
		}
	}

	Vertex3 NormalAtPoint(Vertex3 P) { //return normal
		Vertex3 normal = P - center;
		normal.Unitario();
		return normal;
	}

};

struct VirtualCamera {
	//Camera
	int width, height;
	float pixelSize;
	float fieldOfViewY, fieldOfViewX;
	Vertex3 CamaraCenter;
	Vertex3 CamaraEye;
	Vertex3 up;
	//Objects
	vector<ParametricSphere*> objects;
	//Lights
	vector<Light*> lights;

	VirtualCamera(int width, int height, float pixelSize, float fieldOfViewY, Vertex3 CamEye, Vertex3 CamCenter) {
		this->height = height;
		this->width = width;
		this->pixelSize = pixelSize;
		this->fieldOfViewY = fieldOfViewY;
		fieldOfViewX = (height / width)*fieldOfViewY;
		CamaraEye = CamEye;
		CamaraCenter = CamCenter;
		up.SetVertices(0, 1, 0);
	}

	void Display() {
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

		Vertex3 a, ww, u, v, direccion;
		double alfa, beta;
		a = CamaraEye - CamaraCenter;
		ww = a / a.Magnitud();
		u = (up * ww) / (up * ww).Magnitud();
		v = ww * u;		

		Vertex3 color;
		//ForEachPixel
		for (int w = 0; w < width; w++) {
			for (int h = 0; h < height; h++) {
				//MakeRay
				alfa = tan((fieldOfViewX / 2.0)*(M_PI / 180.0)) * ((w - (width / 2.0)) / (width / 2.0));
				beta = tan((fieldOfViewY / 2.0)*(M_PI / 180.0)) * ((h - (height / 2.0)) / (height / 2.0));
				direccion = ((u*alfa) + (v*beta) - ww)/ ((u*alfa) + (v*beta) - ww).Magnitud();
				//CheckObjectsCollision, Menor
				int closer = -1; double distance = 0;
				for (int i = 0; i < objects.size(); i++) {
					double dist = objects[i]->IntersectionWithRay(CamaraEye, direccion);
					if (dist > 0) {
						closer = i;
						distance = dist;
					}
				}
				//Color
				if (closer >= 0) {
					Vertex3 Point = CamaraEye + direccion * distance;					
					Vertex3 Normal = objects[closer]->NormalAtPoint(Point);
					Vertex3 SourceLightOut = lights[0]->GetPosition() - Point;  SourceLightOut.Unitario();
					Vertex3 SourceLightIn = Point - lights[0]->GetPosition();  SourceLightIn.Unitario();
					Vertex3 Viewer = Point - CamaraEye; Viewer.Unitario();
					Vertex3 Reflected = (Normal * (SourceLightIn & Normal) * 2) - SourceLightIn; //Reflected.Unitario();

					//Direct Light?
					bool directLight = true;
					/*direccion = lights[0]->GetPosition() - Point; direccion.Unitario();
					for (int i = 0; i < objects.size(); i++) {
						double dist = objects[i]->IntersectionWithRay(Point, direccion);
						if (dist > 0.01) {
							directLight = false;
						}
					}*/

					//Ambient
					Vertex3 amb = objects[closer]->material.GetAmbient() | lights[0]->GetAmbient();
					//Diffuse
					double lambert = SourceLightOut & Normal; if (lambert < 0 || !directLight) lambert = 0;
					Vertex3 dif = (objects[closer]->material.GetDiffuse() | lights[0]->GetDiffuse()) * lambert;
					//Specular					
					double phong = Reflected & Viewer; if (phong < 0 || !directLight) phong = 0;
					phong = pow(phong, objects[closer]->material.GetShinnes());
					Vertex3 spec = (objects[closer]->material.GetSpecular() | lights[0]->GetSpecular()) * phong;

					//Calc
					color = amb + dif + spec;
				} else {
					color.SetVertices(0, 0, 0);
				}

				glColor3f(color.GetX(), color.GetY(), color.GetZ());
				DrawQuad(Vertex2(w*pixelSize, h*pixelSize), pixelSize);
			}
		}
	}



	void DrawQuad(Vertex2 point, float size) {
		glBegin(GL_QUADS);
		glVertex2f(point.GetX() + size, point.GetY() + size);
		glVertex2f(point.GetX() - size, point.GetY() + size);
		glVertex2f(point.GetX() - size, point.GetY() - size);
		glVertex2f(point.GetX() + size, point.GetY() - size);
		glEnd();
	}
};