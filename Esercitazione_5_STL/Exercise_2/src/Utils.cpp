#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include <fstream>
#include <sstream>

using namespace std;
namespace MeshLibrary{
double tolsistem =numeric_limits<double>::epsilon(); //imposto come soglia massima
bool controlCell1Ds (const int a, const int b, map<unsigned int, Vector2d> CoordMapsCell0Ds, double tolmesh){
       double tol=max(tolmesh, tolsistem);//massimo tra input e soglia massima
    if((sqrt((CoordMapsCell0Ds[a][0]-CoordMapsCell0Ds[b][0])*(CoordMapsCell0Ds[a][0]-CoordMapsCell0Ds[b][0])+(CoordMapsCell0Ds[a][1]-CoordMapsCell0Ds[b][1])*(CoordMapsCell0Ds[a][1]-CoordMapsCell0Ds[b][1])))<tol){
        return false; //distanza euclidea
    }
    return true;
}
bool controlCell2Ds (vector<unsigned int> Vertices, map<unsigned int, Vector2d> CoordMapsCell0Ds, double tolmesh){
    double previoustol=max(tolmesh, sqrt(tolsistem));
    //faccio il confronto delle radici in modo tale che non ci sia un errore dovuto al calcolo di un numero minore di quello di macchina
    double tol=previoustol*previoustol;//uso la tolleranza di prima al quadrato
    double somma=abs(CoordMapsCell0Ds[Vertices[0]][0]*CoordMapsCell0Ds[Vertices[Vertices.size()-1]][1] - CoordMapsCell0Ds[Vertices[Vertices.size()-1]][0]*CoordMapsCell0Ds[Vertices[0]][1]);
    for (unsigned int i=0; i<Vertices.size()-1; i++){
        somma=abs(CoordMapsCell0Ds[Vertices[i]][0]*CoordMapsCell0Ds[Vertices[i+1]][1] - CoordMapsCell0Ds[Vertices[i+1]][0]*CoordMapsCell0Ds[Vertices[i]][1]) + somma;
    }
    if (0.5*abs(somma)<tol){
        return false;
    }

    return true;
}




bool importMesh (const string& path, PolygonalMesh& mesh){
    cout<<"inserisci un valore di tolleranza da utilizzare nel tuo problema"<<endl;
    cin>>mesh.tol;
    if(!importCell0Ds(path+ "/Cell0Ds.csv", mesh))
        return false;
    else{
        for(auto& el: mesh.VerticesMaker){
            cout<<"key:"<<el.first<<endl;
            for(auto& id: el.second){
                cout<<id<<" ";
            }
            cout<<endl;
        }
    }
    if(!importCell1Ds(path+ "/Cell1Ds.csv", mesh))
        return false;
    else{
        cout<<"elenco dei lati con rispettivi marker"<<endl;
        for(auto& el: mesh.EdgesMaker){
            cout<<"key:"<<el.first<<endl;
            for(auto& id: el.second){
                cout<<id<<" ";
            }
            cout<<endl;
        }
    }
    if(!importCell2Ds(path+ "/Cell2Ds.csv", mesh)){
        return false;
    }
    else{

    }
    return true;

}
bool importCell0Ds (const string& fileName, PolygonalMesh& mesh){
    ifstream file(fileName);
    if (file.fail())
        return false;
    string header;
    getline(file, header);

    list<string> lines;
    string line;
    while (getline(file, line))
        lines.push_back(line);

    file.close();
    unsigned int n=lines.size();
    mesh.IdCell0Ds.reserve(n);
    mesh.MarkerCell0Ds.reserve(n);

    for(const string& line: lines){
        istringstream convert(line);
        unsigned int id;
        unsigned int marker;
        Vector2d coordinates;
        char a;
        convert>>id>>a>>marker>>a>>coordinates(0)>>a>>coordinates(1);//inserimento con punti e virgola in mezzo
        mesh.CoordMapsCell0Ds.insert({id, {coordinates}});
        mesh.IdCell0Ds.push_back(id);
        mesh.MarkerCell0Ds.push_back(marker);
        if (marker !=0){

            auto ret=mesh.VerticesMaker.insert({marker, {id}});
            ((ret.first)) -> second.push_back(id);
        }
    }
    return true;

}
bool importCell1Ds(const string& fileName, PolygonalMesh& mesh) {
    cout<<"elenco dei lati rifiutati"<<endl;
    ifstream file(fileName);
    if (file.fail())
        return false;
    string header;
    getline(file, header);

    list<string> lines;
    string line;
    while (getline(file, line))
        lines.push_back(line);

    file.close();
    unsigned int n=lines.size();

    array<unsigned int,2> Vertices;
    mesh.IdCell0Ds.reserve(n);
    mesh.MarkerCell0Ds.reserve(n);

    for(const string& line: lines){
        istringstream convert(line);
        unsigned int id;
        unsigned int marker;
        char a;
        convert>>id>>a>>marker>>a>>Vertices[0]>>a>>Vertices[1];
        if (controlCell1Ds(Vertices[0], Vertices[1], mesh.CoordMapsCell0Ds, mesh.tol)){
        mesh.VerticesCell1ds.push_back(Vertices);
        mesh.IdCell1Ds.push_back(id);
        mesh.MarkerCell1Ds.push_back(marker);
        if (marker !=0){

            auto ret=mesh.EdgesMaker.insert({marker, {id}});
            if(!ret.second)
                ((ret.first)) -> second.push_back(id);
        }
        }
        else
            cout<<"non esiste il lato "<<id<<endl;
    }

    return true;
}
bool importCell2Ds(const string& fileName, PolygonalMesh& mesh) {
    ifstream file(fileName);
    if (file.fail()){
        return false;
    }
    string header;
    getline(file, header);

    list<string> lines;
    string line;
    while (getline(file, line)){
        lines.push_back(line);
    }

    file.close();
    unsigned int n=lines.size();
    mesh.IdCell2Ds.reserve(n);
    mesh.EdgesCell2Ds.reserve(n);
    mesh.VerticesCell2Ds.reserve(n);


    cout<< "elenco delle aree rifiutate"<<endl;
    for(const string& line: lines){
        istringstream convert(line);
        unsigned int id;
        char a;
        unsigned int cont;
        unsigned int m;
        convert>>id>>a>>m>>a>>cont>>a;//il marker è sempre 0 quindi lo posso non considerare
        vector<unsigned int> Vertices;//creo già un vettore della lunghezza che mi serve
        Vertices.resize(cont);
        for (unsigned int i=0; i<cont; i++){
            convert>>Vertices[i]>>a;//lo inizializzo

        }


        if (controlCell2Ds(Vertices, mesh.CoordMapsCell0Ds, mesh.tol)) { //controllo sull'area

            mesh.VerticesCell2Ds.push_back(Vertices);
            convert>>id>>a>>cont;
            vector<unsigned int> Edges;
            Edges.reserve(cont);
            for (unsigned int i=0; i<cont; i++){
                convert>>Edges[i]>>a;
            }

            mesh.IdCell2Ds.push_back(id);
            mesh.EdgesCell2Ds.push_back(Edges);
        }
        else{
        cout<<"non esiste il "<<id<<endl;
        }



    }

    return true;
}
}
