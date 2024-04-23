#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"

using namespace MeshLibrary;
using namespace std;
int main()
{
    PolygonalMesh mesh;
    string path="./PolygonalMesh"; //path è la cartella
    if(!importMesh(path, mesh)){
        return 1;
    }
    return 0;
}
