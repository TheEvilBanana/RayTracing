#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

float random_num() {
	std::random_device rd;
	float rand = (float)rd() / (float)rd.max();
	return rand;
}

vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2.0f * vec3(random_num(), random_num(), 0) - vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0f);
	return p;
}

class camera {
public:
	vec3 lower_left_corner;
	vec3 hor;
	vec3 ver;
	vec3 origin;
	const float PI = 3.1415927f;
	vec3 u, v, w;
	float lens_radius;

	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist) {
		lens_radius = aperture / 2;
		float theta = vfov * PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = -cross(w, u);

		/*lower_left_corner = vec3(-half_width, -half_height, -1.0f);
		hor = vec3(2 * half_width, 0.0f, 0.0f);
		ver = vec3(0.0f, 2 * half_height, 0.0f);*/
		//origin = vec3(0, 0, 0);
		/*lower_left_corner = origin - half_width * u - half_height * v - w;
		hor = 2 * half_width * u;
		ver = 2 * half_height * v;*/

		lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
		hor = 2 * half_width * focus_dist * u;
		ver = 2 * half_height * focus_dist * v;
	}

	ray get_ray(float s, float t) { 
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin, lower_left_corner + s * hor + t * ver - origin);
	}
};

#endif // !CAMERAH
