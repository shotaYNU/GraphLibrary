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

string PaneledGraph::toSaveGraph()
{
    map<string, picojson::value> data;

    data["genus"] = picojson::value((double)genus);
    data["name"] = picojson::value(name);
    data["orientable"] = picojson::value(orientable);
    data["rotation"] = picojson::value(toRotationString());
    data["faces"] = picojson::value(toFacesString());

    picojson::value v(data);
    std::string jsonString = v.serialize(false);

    return jsonString;
}

string PaneledGraph::toSaveGraph(vector<pair<string, picojson::value> > _withData)
{
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
    std::string jsonString = v.serialize(false);

    return jsonString;
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

void PaneledGraph::setPanel(string _panelString)
{
    vector<PaneledFace*> faces;
    PaneledFace* face;
    EmbeddedEdge *ed, *end;

    initFace();
    for (int i = 0; i < verticesNum; ++i) {
        ed = end = vertices[i]->getFirstEdge();
        do {
            face = (PaneledFace*)ed->getNextFace();
            face->resetPanel();
            if (find(faces.begin(), faces.end(), face) == faces.end())
                faces.push_back(face);
            ed = ed->getNext();
        } while(ed != end);
    }

    vector<string> panels = Utility::split(_panelString, ',');
    for (auto panel : panels) {
        vector<string> ids = Utility::split(panel, ':');
        int id1 = atoi(ids[0].c_str());
        int id2 = atoi(ids[1].c_str());
        int id3 = atoi(ids[2].c_str());

        vector<int> face1 = { id1, id2, id3 };
        sort(face1.begin(), face1.end());
        for (auto face : faces) {
            vector<int> face2 = face->getFace();
            sort(face2.begin(), face2.end());
            if (face1 == face2)
                face->setPanel();
        }
    }
}

void PaneledGraph::openGraph(string _filepath)
{
    ifstream ifs;
    ifs.open(_filepath);

    if (!ifs.is_open()) {
        cerr << "cannot open file!" << endl;
        exit(1);
    }

    stringstream sstream;
    string line;
    while (getline(ifs, line)) {
        sstream << line;
    }
    ifs.close();

    picojson::value v;
    picojson::parse(v, sstream);

    picojson::object& all = v.get<picojson::object>();
    genus = all["genus"].get<double>();
    name = all["name"].get<string>();
    orientable = all["orientable"].get<bool>();
    setRotation(all["rotation"].get<string>());
    if (all.find("panels") != all.end()) {
        setPanel(all["panels"].get<string>());
    } else if (all.find("faces") != all.end()) {
        vector<string> strs = Utility::split(all["faces"].get<string>(), ',');
        string panels = "";
        for (auto str : strs) {
            vector<string> f = Utility::split(str, ':');
            if (f[1] == "1") {
                int id1 = get<0>(EmbeddedVertex::charToId(f[0][0]));
                int id2 = get<0>(EmbeddedVertex::charToId(f[0][1]));
                int id3 = get<0>(EmbeddedVertex::charToId(f[0][2]));
                panels += to_string(id1) + ":";
                panels += to_string(id2) + ":";
                panels += to_string(id3);
                panels += ",";
            }
        }
        panels.pop_back();
        setPanel(panels);
    }
}
