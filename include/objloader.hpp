#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include <fstream>
#include <glm/glm.hpp>
#include <GL/glut.h>
#include <vector>
#include <sstream>
#include <iostream>


struct Vertex
{
    float x;
    float y;
    float z;

    float u;
    float v;

    float nx;
    float ny;
    float nz;

    Vertex(float fx, float fy, float fz, float fu, float fv, float fnx, float fny, float fnz) //TODO: Replace Vertex with builtins
        : x(fx), y(fy), z(fz), u(fu), v(fv), nx(fnx), ny(fny), nz(fnz) {};
    ~Vertex() = default;
};


class OBJLoader
{
   static void LoadVertex(std::vector<glm::vec3>& vec, std::stringstream& strm)
   {
      glm::vec3 vertex;
      strm >> vertex.x;
      strm >> vertex.y;
      strm >> vertex.z;
      vec.push_back(vertex);
   }

   static void LoadUV(std::vector<glm::vec2>& vec, std::stringstream& strm)
   {
      glm::vec2 uv;
      strm >> uv.x;
      strm >> uv.y;
      vec.push_back(uv);
   }

   static void LoadNormal(std::vector<glm::vec3>& vec, std::stringstream& strm)
   {
      glm::vec3 normal;
      strm >> normal.x;
      strm >> normal.y;
      strm >> normal.z;
      vec.push_back(normal);
   }

   static void LoadFace(std::vector<unsigned>& vertexIndex,
                        std::vector<unsigned>& uvIndex,
                        std::vector<unsigned>& normalIndices,
                        std::stringstream& strm)
   {
      unsigned x;
      int count = 0;
      while(strm >> x)
      {
         if(count == 9)
            throw "Why the long face?";

         if(strm.peek() == '/')
            strm.ignore();
         if(count % 3 == 0) vertexIndex.push_back(x);
         else if (count % 3 == 1) uvIndex.push_back(x);
         else normalIndices.push_back(x);
         ++count;
      }
   }

   static void IndexData(std::vector<unsigned>& vertexIndex,
                         std::vector<unsigned>& uvIndex,
                         std::vector<unsigned>& normalIndices,
                         std::vector<glm::vec3>& temp_out_vertices,
                         std::vector<glm::vec2>& temp_out_uvs,
                         std::vector<glm::vec3>& temp_out_normal,
                         std::vector<glm::vec3>& out_vertices,
                         std::vector<glm::vec2>& out_uvs,
                         std::vector<glm::vec3>& out_normal)
   {
      for(unsigned x : vertexIndex)
      {
         out_vertices.push_back(temp_out_vertices[x-1]);
      }
      for(unsigned x : uvIndex)
      {
         out_uvs.push_back(temp_out_uvs[x-1]);
      }
      for(unsigned x : normalIndices)
      {
         out_normal.push_back(temp_out_normal[x-1]);
      }
   }

public:
    static bool LoadOBJFromFile(std::string filepath,std::vector<Vertex>& dest)
   {
      std::ifstream objFile(filepath);
      if(!objFile.is_open())
      {
         std::cout << "Unable to open file!";
         return false;
      }
      std::vector<unsigned> vertexIndices;
      std::vector<unsigned> uvIndices;
      std::vector<unsigned> normalIndices;

      std::vector<glm::vec3> temp_out_vertices;
      std::vector<glm::vec2> temp_out_uvs;
      std::vector<glm::vec3> temp_out_normal;


      std::vector<glm::vec3> out_vertices;
      std::vector<glm::vec2> out_uvs;
      std::vector<glm::vec3> out_normal;

      std::string line;
      std::string token;
      while(getline(objFile, line))
      {
         std::stringstream ss;
         ss << line;
         ss >> token;
         if(token == "#") continue;
         else if (token == "s") continue;
         else if (token == "v") LoadVertex(temp_out_vertices, ss);
         else if (token == "vt") LoadUV(temp_out_uvs, ss);
         else if (token == "vn") LoadNormal(temp_out_normal, ss);
         else if (token == "f") LoadFace(vertexIndices, uvIndices, normalIndices, ss);
      }
      IndexData(vertexIndices, uvIndices, normalIndices,
                temp_out_vertices, temp_out_uvs, temp_out_normal,
                out_vertices, out_uvs, out_normal);
      for(unsigned int i = 0; i<out_vertices.size(); i++ )
      {

          dest.push_back(Vertex(out_vertices[i].x, out_vertices[i].y, out_vertices[i].z, out_uvs[i].x, out_uvs[i].y, out_normal[i].x, out_normal[i].y, out_normal[i].z));
      }

      return true;
   }

/*    static bool LoadOBJ(std::string filepath, std::vector<glm::vec3> vertices)
   {
      std::vector<glm::vec2> uvs;
      std::vector<glm::vec3> normal;
      bool status = LoadOBJFromFile(filepath, vertices, uvs, normal);
      return status;
      }*/
};

#endif //OBJLOADER_HPP
