#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream> 

bool sameSide(const point3& vertex1, const point3& vertex2, const point3& vertex3, const vec3& hit) {
    vec3 v1 = vertex2 - vertex3;
    vec3 a1 = cross(v1, (hit - vertex3));
    vec3 b1 = cross(v1, (vertex1 - vertex3));
    double c1 = dot(a1, b1);

    vec3 v2 = vertex1 - vertex2;
    vec3 a2 = cross(v2, (hit - vertex2));
    vec3 b2 = cross(v2, (vertex3 - vertex2));
    double c2 = dot(a2, b2);

    vec3 v3 = vertex2 - vertex1;
    vec3 a3 = cross(v3, (hit - vertex1));
    vec3 b3 = cross(v3, (vertex3 - vertex1));
    double c3 = dot(a3, b3);
    
    if (c1 >= 0 && c2 >= 0  && c3 >= 0) {
        return true;
    }

    return false;
}


bool hit_triangle(const ray& r, const point3& vertex1, const point3& vertex2, const point3& vertex3) {
    vec3 planeVector1 = vertex2 - vertex1;
    vec3 planeVector2 = vertex3 - vertex1;

    vec3 normal = cross(planeVector1, planeVector2);
    double k = dot(-normal, vertex1);

    double t = -(dot(normal, r.origin()) + k) / dot(normal, r.direction());
    
    //hit not in view
    if (t < .0) {
        return false;
    }

    point3 hit = r.at(t);
    
    //hit not in the triangle bounding box
    auto minX = std::min(std::min(vertex1[0], vertex2[0]), vertex3[0]);
    auto minY = std::min(std::min(vertex1[1], vertex2[1]), vertex3[1]);
    auto minZ = std::min(std::min(vertex1[2], vertex2[2]), vertex3[2]);
    auto maxX = std::max(std::max(vertex1[0], vertex2[0]), vertex3[0]);
    auto maxY = std::max(std::max(vertex1[1], vertex2[1]), vertex3[1]);
    auto maxZ = std::max(std::max(vertex1[2], vertex2[2]), vertex3[2]);
    if (!(minX <= hit[0] && hit[0] <= maxX) || !(minY <= hit[1] && hit[1] <= maxY) || !(minZ <= hit[2] && hit[2] <= maxZ)) {
        return false;
    }
    
    //hit not in the triangle
    if (!sameSide(vertex1, vertex2, vertex3, hit)) {
        return false;
    }

    return true;
}

color ray_color(const ray& r) {
    vec3 vertex1 = vec3(-1.0, 0.0, 2.0);
    vec3 vertex2 = vec3(0.0, -1.0, 3.0);
    vec3 vertex3 = vec3(1.0, 0.0, 2.0);
    if (hit_triangle(r, vertex1, vertex2, vertex3)) {
        return color(1.0, 1.0, 1.0);
    }
    else {
        /*vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);*/
        return color(0, 0, 0);
    }
    
}

int main() {

    // Image

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = point3(0, 0, 5);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center
        - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render

    vec3 vertex1 = vec3(-1.0, 1.0, 2.0);
    vec3 vertex2 = vec3(0.0, -1.0, 3.0);
    vec3 vertex3 = vec3(1.0, 1.0, 2.0);
    ray r = ray(camera_center, vec3(0, 0, 1));
    std::clog << hit_triangle(r, vertex1, vertex2, vertex3) << std::endl;

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}