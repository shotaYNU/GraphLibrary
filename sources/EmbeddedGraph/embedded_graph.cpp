#include "embedded_graph.hpp"

EmbeddedGraph::EmbeddedGraph()
{
    verticesNum = 0;
    edgesNum = 0;
    firstverticiesNum = 0;
    initializedVerticesNum = 0;
    orientable = false;
    genus = -1;
    name = "";
}

EmbeddedGraph::~EmbeddedGraph()
{
  /*  for (int i = 0; i < verticesNum; ++i)
        delete vertices[i];*/
}

/*****    Add a vertex.    *****/
EmbeddedVertex* EmbeddedGraph::addVertex()
{
    if (verticesNum >= initializedVerticesNum) {
        vertices[verticesNum] = new EmbeddedVertex(verticesNum);
        verticesNum++;
        initializedVerticesNum++;
    } else {
        vertices[verticesNum]->initAdjacent();
        vertices[verticesNum]->setIsContractible(false);
        vertices[verticesNum]->initContractibleEdgesNum();
        vertices[verticesNum]->setId(verticesNum);
        verticesNum++;
    }

    return vertices[verticesNum - 1];
}

/*****    Delete a vertex.    *****/
void EmbeddedGraph::deleteVertex()
{
    verticesNum--;
}

EmbeddedEdge* EmbeddedGraph::makeEdge(int _id1, int _id2, bool _flip) {
    EmbeddedEdge *ed1 = new EmbeddedEdge();
    EmbeddedEdge *ed2 = new EmbeddedEdge();
    EmbeddedEdge *ed1Op = new EmbeddedEdge();
    EmbeddedEdge *ed2Op = new EmbeddedEdge();

    ed1->setOpposite(ed1Op); ed2->setOpposite(ed2Op);
    ed1Op->setOpposite(ed1); ed2Op->setOpposite(ed2);

    ed1->setStart(_id1); ed1->setEnd(_id2);
    ed2->setStart(_id2); ed2->setEnd(_id1);

    ed1->setInverse(ed2, _flip); ed2->setInverse(ed1, _flip);

    edgesNum += 2;

    return ed1;
}

/*****    Set a graph data from json data.    *****/
void EmbeddedGraph::openGraph(string _filepath)
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
}

void EmbeddedGraph::setRotation(string _rotation)
{
    verticesNum = 0; edgesNum = 0;

    vector<string> adjacents = Utility::split(_rotation, ',');

    int id1 = 0, id2;
    bool upperFlag = false;
    bool isFirst;
    EmbeddedEdge *prevEdge, *newEdge, *firstEdge;
    EmbeddedEdge *eds[GraphProperty::MAX_VERTEX][GraphProperty::MAX_VERTEX];
    tuple<int, bool> ids;
    bool flip;

    for (int i = 0; i < adjacents.size(); ++i)
        addVertex();

    for(string adj : adjacents) {
        isFirst = true;
        for (char ch : adj) {
            if (ch == '#') {
                upperFlag = true;
                continue;
            }
            ids = EmbeddedVertex::charToId(ch);
            id2 = get<0>(ids); flip = get<1>(ids);
            if (upperFlag)
                id2 += 26;

            if (id1 < id2) {
                eds[id1][id2] = newEdge = makeEdge(id1, id2, flip);
                eds[id2][id1] = (!flip) ? newEdge->getInverse() : newEdge->getOpposite()->getInverse();
                vertices[id1]->setAdjacent(id2);
                vertices[id2]->setAdjacent(id1);
            } else {
                newEdge = eds[id1][id2];
            }
            if (isFirst) {
                vertices[id1]->setFirstEdge(newEdge);
                firstEdge = newEdge;
                isFirst = false;
            } else {
                prevEdge->setNext(newEdge);
                newEdge->setPrev(prevEdge);
            }
            prevEdge = newEdge;
        }
        prevEdge->setNext(firstEdge);
        firstEdge->setPrev(prevEdge);
        vertices[id1]->setDegree((int)adj.size());
        id1++;
    }

    firstverticiesNum = verticesNum;
    initContractible();
}

/*****    Show a graph data to console.(Clockwise order)    *****/
void EmbeddedGraph::viewClockwiseOrder() const
{
    EmbeddedEdge *now, *start;

    for (int i = 0; i < verticesNum; ++i) {
        if (vertices[i]->getDegree() == 0) {
            cout << i << ":none" << endl;
            continue;
        }

        start = vertices[i]->getFirstEdge();
        cout << start->getStart() << "." << start->getEnd() << "(" << start->getIsContractible() << ")" << " ";
        for (now = start->getNext(); now != start; now = now->getNext())
            cout << now->getEnd() << "(" << now->getIsContractible() << ")" << " ";
        cout << endl;
    }
    cout << endl;
}

/*****    Show a graph data to console.(UntiClockwise order)    *****/
void EmbeddedGraph::viewUntiClockwiseOrder() const
{
    EmbeddedEdge *now, *start;

    for (int i = 0; i < verticesNum; ++i) {
        start = vertices[i]->getFirstEdge();
        cout << start->getStart() << "." << start->getEnd() << "(" << start->getIsContractible() << ")" << " ";
        for (now = start->getPrev(); now != start; now = now->getPrev())
            cout << now->getEnd() << "(" << now->getIsContractible() << ")" << " ";
        cout << endl;
    }
    cout << endl;
}

// Save graph data to text data.
// Example: K4
//      vertex 0's rotation is "1 3 2"
//      vertex 1's rotation is "2 3 0"
//      vertex 2's rotation is "0 3 1"
//      vertex 3's rotation is "0 1 2"
// => rotation: "bdc,cda,adb,abc"
void EmbeddedGraph::saveGraph(string _filepath)
{
    ofstream outfile(_filepath);
    map<string, picojson::value> data;

    data["genus"] = picojson::value((double)genus);
    if (firstverticiesNum == verticesNum)
        data["name"] = picojson::value(name);
    else
        data["name"] = picojson::value("by_" + name);
    data["orientable"] = picojson::value(orientable);
    data["rotation"] = picojson::value(toRotationString());

    picojson::value v(data);
    std::string json_str = v.serialize(false);

    outfile << json_str;
    outfile.close();
}

string EmbeddedGraph::toRotationString() const
{
    string rotation_str = "";
    EmbeddedEdge *ed, *end;
    char id;
    tuple<char, bool> idValue;
    bool upper;

    EmbeddedEdge *start, *now;
    for (int i = 0; i < verticesNum; ++i) {
        start = vertices[i]->getFirstEdge();
        start->unmark(); start->getOpposite()->unmark();
        for (now = start->getNext(); now != start; now = now->getNext()) {
            now->unmark();
            now->getOpposite()->unmark();
        }
    }

    for (int i = 0; i < verticesNum; ++i) {
        ed = end = vertices[i]->getFirstEdge();
        do {
            ed->mark();
            ed = ed->getNext();
        } while (ed != end);
    }

    for (int i = 0; i < verticesNum; ++i) {
        ed = end = vertices[i]->getFirstEdge();
        do {
            id = (char)ed->getEnd();
            bool flip = false;
            if (ed->getMark() != ed->getInverse()->getMark()) {
                flip = true;
            }
            idValue = EmbeddedVertex::idToChar(id, flip);
            id = get<0>(idValue); upper = get<1>(idValue);
            if (upper) rotation_str += '#';
            rotation_str += id;
            ed = ed->getNext();
        } while (ed != end);
        if (i != verticesNum - 1) rotation_str += ',';
    }

    return rotation_str;
}

//Set all contractible edges.
void EmbeddedGraph::initContractible()
{
    EmbeddedEdge *ed, *end;

    for (int i = 0; i < verticesNum; ++i) {
        vertices[i]->initContractibleEdgesNum();
        vertices[i]->setIsContractible(false);
        ed = end = vertices[i]->getFirstEdge();
        do {
            if (vertices[i]->getCommonCount(vertices[ed->getEnd()]) == 2) {
                ed->setIsContractible(true);
                vertices[i]->increaseContractibleEdgesNum();
                vertices[i]->setIsContractible(true);
            } else ed->setIsContractible(false);
            ed = ed->getNext();
        } while (ed != end);
    }
}

//Set all faces.
void EmbeddedGraph::initFace()
{
    EmbeddedEdge *ed1, *ed2, *end;
    int count;

    for (int i = 0; i < verticesNum; ++i) {
        ed1 = end = vertices[i]->getFirstEdge();
        do {
            ed2 = ed1;
            count = 0;
            do {
                count++;
                ed2 = ed2->getInverse()->getPrev();
            } while (ed2 != ed1);

            ed1->getRightFace()->setFaceVerticesNum(count);
            ed1 = ed1->getNext();
        } while (ed1 != end);
    }
}

void EmbeddedGraph::insertEdges(EmbeddedEdge* _start, vector<EmbeddedEdge*> _insertEdges)
{
    EmbeddedEdge* nowEdge = _start;
    for (EmbeddedEdge* newEdge : _insertEdges) {
        insertEdge(nowEdge, newEdge);
        nowEdge = newEdge;
    }
}

void EmbeddedGraph::insertEdge(EmbeddedEdge* _start, EmbeddedEdge* _insertEdge)
{
    EmbeddedEdge* end = _start->getNext();

    _start->setNext(_insertEdge);
    _insertEdge->setPrev(_start);
    _insertEdge->setNext(end);
    end->setPrev(_insertEdge);

    int id = _start->getStart();
    vertices[id]->setFirstEdge(_start);
    vertices[id]->increaseDegree(1);

    int id2 = _insertEdge->getEnd();
    vertices[id]->setAdjacent(id2);
}

void EmbeddedGraph::removeEdges(EmbeddedEdge* _start, EmbeddedEdge* _end, EmbeddedEdge** _removedEdges)
{
    int count = 0;
    EmbeddedEdge *now = _start->getNext();

    while (now != _end) {
        removeEdge(now->getInverse());
        _removedEdges[count++] = now;
        _removedEdges[count++] = now->getInverse();

        vertices[now->getStart()]->resetAdjacent(now->getEnd());
        vertices[now->getStart()]->increaseDegree(-1);

        now = now->getNext();
    }

    _start->setNext(_end);
    _end->setPrev(_start);

    int id = _start->getStart();
    vertices[id]->setFirstEdge(_start);
}

void EmbeddedGraph::removeEdges(EmbeddedEdge* _start, int _count, EmbeddedEdge** _removedEdges)
{
    EmbeddedEdge *end = _start;
    for (int i = 0; i <= _count; ++i)
        end = end->getNext();

    removeEdges(_start, end, _removedEdges);
}

void EmbeddedGraph::removeEdge(EmbeddedEdge* _ed)
{
    EmbeddedEdge *prev = _ed->getPrev();
    EmbeddedEdge *next = _ed->getNext();

    prev->setNext(next);
    next->setPrev(prev);

    int id = next->getStart();
    vertices[id]->setFirstEdge(next);

    int id2 = _ed->getEnd();
    vertices[id]->resetAdjacent(id2);
    vertices[id]->increaseDegree(-1);

    edgesNum -= 2;
}
