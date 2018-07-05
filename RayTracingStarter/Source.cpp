#include <iostream>
#include <random>
//#include "vec3.h"
#include "sphere.h"
#include "entity_list.h"
#include "camera.h"
#include "material.h"



vec3 color(const ray& r, entity *world, int depth) {

	hit_record rec;
	if (world->hit(r, 0.001f, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
		//vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		//return 0.5*color(ray(rec.p, target - rec.p), world);
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f*(unit_direction.y() + 1.0f);
		return (((1.0f - t)* vec3(1.0f, 1.0f, 1.0f)) + (t * vec3(0.5f, 0.7f, 1.0f)));
	}

}

entity *random_scene() {
	int n = 500;
	entity **list = new entity*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5f, 0.5f, 0.5f)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = random_num();
			vec3 center(a + 0.9f * random_num(), 0.2f, b + 0.9f * random_num());
			if ((center - vec3(4, 0.2f, 0)).length() > 0.9f) {
				if (choose_mat < 0.8f) {  // diffuse
					list[i++] = new sphere(center, 0.2f, new lambertian(vec3(random_num()*random_num(), random_num()*random_num(), random_num()*random_num())));
				}
				else if (choose_mat < 0.95f) { // metal
					list[i++] = new sphere(center, 0.2f,
						new metal(vec3(0.5f * (1 + random_num()), 0.5f * (1 + random_num()), 0.5f * (1 + random_num())), 0.5f * random_num()));
				}
				else {  // glass
					list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5f));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4f, 0.2f, 0.1f)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0));

	return new entity_list(list, i);
}

int main() {
	int nx = 1200;
	int ny = 800;
	int ns = 25;

	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	const float PI = 3.1415927;
	//float R = cos(PI / 4);
	entity* list[5];
	list[0] = new sphere(vec3(0, 0, -1), 0.5f, new lambertian(vec3(0.1f, 0.2f, 0.5f)));
	list[1] = new sphere(vec3(0, -100.5f, -1), 100, new lambertian(vec3(0.8f, 0.8f, 0.0f)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.0f));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5f, new dielectric(1.5f));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45f, new dielectric(1.5f));

	//list[0] = new sphere(vec3(-R, 0, -1), R, new lambertian(vec3(0, 0, 1)));
	//list[1] = new sphere(vec3(R, 0, -1), R, new lambertian(vec3(1, 0, 0)));

	entity* world = new entity_list(list, 5);
	world = random_scene();

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	//float dist_to_focus = (lookfrom - lookat).length();
	float dist_to_focus = 10.0f;
	float aperture = 0.0f;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 45, float(nx)/float(ny), aperture, dist_to_focus);

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + random_num()) / float(nx);
				float v = float(j + random_num()) / float(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0f);
				col += color(r, world, 0);
			}
			
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";

		}
	}
}