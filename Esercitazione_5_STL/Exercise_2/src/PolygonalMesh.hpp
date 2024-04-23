#pragma once
#include <iostream>
#include <vector>
#include<Eigen/Eigen>
using namespace std;
using namespace Eigen;
namespace MeshLibrary{


struct PolygonalMesh{
    unsigned int NumberofCell0Ds;
    vector<unsigned int> IdCell0Ds;
    map<unsigned int, Vector2d> CoordMapsCell0Ds; //mappa per accedere tramite id (non mi fido di vettore con posizione id)
    vector<unsigned int> MarkerCell0Ds;

    unsigned int NumberofCell1Ds;
    vector<unsigned int> IdCell1Ds;
    vector<array<unsigned int, 2>> VerticesCell1ds;
    vector<unsigned int> MarkerCell1Ds;

    vector<unsigned int> IdCell2Ds;
    vector<vector<unsigned int>> VerticesCell2Ds;
    vector<vector<unsigned int>> EdgesCell2Ds;

    map<unsigned int, list<unsigned int>> VerticesMaker;

    map<unsigned int, list<unsigned int>> EdgesMaker;

    double tol;


};

}
