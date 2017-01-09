#include "paneled_graph.hpp"

PaneledGraph::PaneledGraph()
{

}

PaneledGraph::~PaneledGraph()
{

}

bool PaneledGraph::isPIT()
{
    initContractible();
    bool isIrreducible = true;
    for (int i = 0; i < verticesNum; ++i)
        if (vertices[i]->getIsContractible()) {
            isIrreducible = false;
            break;
        }
    if (isIrreducible) return true;
    setPanelsWithAllEmbeddings();
    for (int i = 0; i < verticesNum; ++i)
        if (vertices[i]->getIsContractible() && isFlat(i))
            return false;

    return true;
}

bool PaneledGraph::isFlat(int _id)
{
    EmbeddedEdge *ed, *end;

    ed = end = vertices[_id]->getFirstEdge();
    do {
        if (!((PaneledFace*)ed->getNextFace())->getPanel())
            return false;
        ed = ed->getNext();
    } while(ed != end);

    return true;
}

void PaneledGraph::saveGraph(string _filepath)
{
    ofstream outfile(_filepath);
    map<string, picojson::value> data;

    data["genus"] = picojson::value((double)genus);
    data["name"] = picojson::value(name);
    data["orientable"] = picojson::value(orientable);
    data["rotation"] = picojson::value(toRotationString());
    data["faces"] = picojson::value(toFacesString());

    picojson::value v(data);
    std::string json_str = v.serialize(false);

    outfile << json_str;
    outfile.close();
}


void PaneledGraph::saveGraph(string _filepath, vector<pair<string, picojson::value>> _withData)
{
    ofstream outfile(_filepath);
    map<string, picojson::value> data;

    data["genus"] = picojson::value((double)genus);
    data["name"] = picojson::value(name);
    data["orientable"] = picojson::value(orientable);
    data["rotation"] = picojson::value(toRotationString());
    data["faces"] = picojson::value(toFacesString());
    for (auto onedata : _withData) {
        string key = get<0>(onedata);
        picojson::value value = get<1>(onedata);
        data[key] = value;
    }

    picojson::value v(data);
    std::string json_str = v.serialize(false);

    outfile << json_str;
    outfile.close();
}

string PaneledGraph::toFacesString()
{
    vector<PaneledFace*> faces;
    PaneledFace* face;
    EmbeddedEdge *ed, *end;

    initFace();
    for (int i = 0; i < verticesNum; ++i) {
        ed = end = vertices[i]->getFirstEdge();
        do {
            face = (PaneledFace*)ed->getNextFace();
            if (find(faces.begin(), faces.end(), face) == faces.end())
                faces.push_back(face);
            ed = ed->getNext();
        } while(ed != end);
    }

    string facesString = "";
    for (auto f : faces) {
        for (auto v : f->getFace())
            facesString += get<0>(EmbeddedVertex::idToChar(v, false));
        facesString += ":";
        facesString += f->getPanel() ? "1" : "0";
        facesString += ",";
    }
    facesString.pop_back();

    return facesString;
}

void PaneledGraph::setPanelsWithAllEmbeddings()
{
    vector<PaneledFace*> faces;
    vector<long long> originFaces;
    vector<long long> newFaces;
    PaneledFace* face;
    EmbeddedEdge *ed, *end;

    initFace();
    for (int i = 0; i < verticesNum; ++i) {
        ed = end = vertices[i]->getFirstEdge();
        do {
            face = (PaneledFace*)ed->getNextFace();
            if (find(faces.begin(), faces.end(), face) == faces.end()) {
                face->setPanel();
                faces.push_back(face);
            }
            ed = ed->getNext();
        } while(ed != end);
    }

    for (auto face : faces) {
        originFaces.push_back(0ULL);
        newFaces.push_back(0ULL);
        for (auto vertex : face->getFace()) {
            originFaces.back() += (1ULL << vertex);
            newFaces.back() += (1ULL << vertex);
        }
    }

    embeddingSet.setAllEmbeddings(this);
    for (int e = 0; e < embeddingSet.getInequivalentEmbeddingsNum(); ++e) {
        newFaces = originFaces;
        for (auto transposition : embeddingSet.getInequivalentEmbeddings(e))
            Utility::allExchange(transposition.first, transposition.second, newFaces);
        for (int i = 0; i < faces.size(); ++i) {
            if (!faces[i]->getPanel()) continue;
            if (find(newFaces.begin(), newFaces.end(), originFaces[i]) == newFaces.end())
                faces[i]->resetPanel();
        }
    }
}
