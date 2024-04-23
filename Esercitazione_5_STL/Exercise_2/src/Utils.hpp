#pragma once
#include <iostream>
#include "PolygonalMesh.hpp"
using namespace std;
namespace MeshLibrary{
bool importMesh (const string& path, PolygonalMesh& mesh);

bool importCell0Ds (const string& fileName, PolygonalMesh& mesh);

bool importCell1Ds (const string& fileName, PolygonalMesh& mesh);

bool importCell2Ds (const string& fileName, PolygonalMesh& mesh);

bool controlCell1Ds (const int a, const int b, map<unsigned int, Vector2d> CoordMapsCell0Ds, double tol);
//controllo se la tolleranza è rispettata

bool controlCell2Ds (vector<unsigned int> Vertices, map<unsigned int, Vector2d> CoordMapsCell0Ds, double tol);
}

