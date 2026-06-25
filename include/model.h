#ifndef MODEL_H
#define MODEL_H

#include "hittable_list.h"
#include "material.h"
#include "tiny_obj_loader.h"
#include "rtweekend.h"
#include "triangle.h"
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

inline hittable_list load_model(const std::string &filename,
                                shared_ptr<material> mat) {
  hittable_list model;

  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::string warn;
  std::string err;

  std::string mtldir = filename;
  auto sep = mtldir.find_last_of("/\\");
  if (sep != std::string::npos)
    mtldir = mtldir.substr(0, sep + 1);

  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                              filename.c_str(), mtldir.c_str());

  if (!warn.empty())
    std::cerr << "WARN: " << warn << "\n";
  if (!err.empty())
    std::cerr << "ERR: " << err << "\n";
  if (!ret) {
    std::cerr << "Failed to load: " << filename << "\n";
    return model;
  }

  size_t tri_count = 0;
  for (const auto &shape : shapes) {
    size_t index_offset = 0;
    for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
      int fv = shape.mesh.num_face_vertices[f];

      if (fv == 3) {
        tinyobj::index_t idx0 = shape.mesh.indices[index_offset + 0];
        tinyobj::index_t idx1 = shape.mesh.indices[index_offset + 1];
        tinyobj::index_t idx2 = shape.mesh.indices[index_offset + 2];

        vec3 v0(attrib.vertices[3 * idx0.vertex_index + 0],
                attrib.vertices[3 * idx0.vertex_index + 1],
                attrib.vertices[3 * idx0.vertex_index + 2]);
        vec3 v1(attrib.vertices[3 * idx1.vertex_index + 0],
                attrib.vertices[3 * idx1.vertex_index + 1],
                attrib.vertices[3 * idx1.vertex_index + 2]);
        vec3 v2(attrib.vertices[3 * idx2.vertex_index + 0],
                attrib.vertices[3 * idx2.vertex_index + 1],
                attrib.vertices[3 * idx2.vertex_index + 2]);

        model.add(make_shared<triangle>(v0, v1, v2, mat));
        tri_count++;
      }

      index_offset += fv;
    }
  }

  std::cerr << "Loaded " << tri_count << " triangles from " << filename << "\n";
  return model;
}

#endif // !MODEL_H
