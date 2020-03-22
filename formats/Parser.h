#include "x3dElems.h"
#include "Sphere.h"
#include <vector>
#include <map>
#include <stack>
#include <string>
#include "tinyxml2.h"


class Parser {
public:
    /**
     * This method used to create parser and parse xml file.
     * @param name - path to xml file
     */
    Parser(const char* name) {
        skipHTML(name);
        int d = 0;
        std::stack<Transformationx3d> tab;
        createGetList();
    }
    /**
    * @return true if you can take one more object
    */
    bool hasNext() {
        return !listToPop.empty();
    }
    /**
     * this method return next parsed object from xml file
     * @return shape taken from xml file.
     */
    BasicShape * NextElem() {
        BasicShape * s = new BasicShape();
        if (hasNext()) {
            *s = listToPop.top();
            listToPop.pop();
        }
        return s;
    }

private:
    std::stack<Shapex3d> list;
    std::stack<BasicShape> listToPop;
    int pos = 0;
    std::map<std::string, Materialx3d> material_map;
    std::map<std::string, Appearancex3d> appearance_map;
    std::map<std::string, Transformationx3d> transformation_map;

    void skipHTML(const char* fileName) {
        XMLDocument xml_doc;
        XMLError eResult = xml_doc.LoadFile(fileName);
        if (eResult != XML_SUCCESS) {
            //  printf("Incorrect file!\n");
            return;
        }
        XMLNode* xml = xml_doc.FirstChild();
        if (xml == nullptr) {
            return;
        }
        while (strcmp(xml->Value(), "xml") != 0 && strcmp(xml->Value(), "html") != 0 && strcmp(xml->Value(), "x3d") != 0 &&
            strcmp(xml->Value(), "X3D") != 0) {
            xml = xml->NextSibling();
            if (xml == nullptr) {
                return;
            }
        }
        XMLNode* Node;
        if (strcmp(xml->Value(), "xml") == 0 || strcmp(xml->Value(), "html") == 0) {
            XMLNode* body;
            body = xml->FirstChildElement("body");
            if (body == nullptr) {
                return;
            }
            Node = body->FirstChildElement("X3D");
            if (Node == nullptr) {
                Node = body->FirstChildElement("x3d");
                if (Node == nullptr) {
                    return;
                }
            }
        }
        else {
            Node = xml;
        }
        Node = Node->FirstChildElement("Scene");
        if (Node != nullptr)
            Node = Node->FirstChild();
        if (Node != nullptr)
            NodeParser(Node);
    }

    void NodeParser(XMLNode* Node) {
        if (strcmp(Node->Value(), "Group") == 0)
            Node = Node->NextSibling();
        while (Node) {
            // printf("%s\n", Node->Value());
            if (strcmp(Node->Value(), "Group") == 0)
                NodeParser(Node->FirstChild());
            else if (strcmp(Node->Value(), "Transform") == 0) {
                TransformParser(nullptr, Node);
            }
            Node = Node->NextSibling();
        }
    }

    //parse field transform
    void TransformParser(Transformationx3d* T, XMLNode* Node) {
        XMLElement* node = Node->ToElement();
        if (node == nullptr) {
            return;
        }
        int numArgs = 0;
        auto* newTrans = new Transformationx3d();
        if ((numArgs = findAtribute(newTrans->translation, 3, "translation", node)) == 0)
            for (float& i : newTrans->translation)
                i = 0;
        else if (numArgs != 3)
            for (float& i : newTrans->translation)
                i = 0;

        if ((numArgs = findAtribute(newTrans->rotation, 4, "rotation", node)) == 0) {
            for (float& i : newTrans->rotation)
                i = 0;
            newTrans->rotation[2] = -5;
        }

        if ((numArgs = findAtribute(newTrans->scale, 3, "scale", node)) == 0) {
            for (float& i : newTrans->scale)
                i = 0;
        }
        else if (numArgs != 3)
            for (float& i : newTrans->scale)
                i = 0;
        if (T != nullptr) {
            for (int i = 0; i < 3; ++i) {
                newTrans->rotation[i] += T->rotation[i];
                newTrans->translation[i] += T->translation[i];
            }
            for (int i = 0; i < 4; ++i) {
                newTrans->scale[i] += T->scale[i];
            }
        }
        XMLNode* subNode = Node->FirstChild();
        while (subNode != nullptr) {
            if (strcmp(subNode->Value(), "Transform") == 0) {
                TransformParser(newTrans, subNode);
            }
            if (strcmp(subNode->Value(), "Group") == 0) { /*Groupx3d(); continue;*/ }
            if (strcmp(subNode->Value(), "Shape") == 0) {
                ShapeParser(newTrans, subNode);
            }
            subNode = subNode->NextSibling();
        }
        if (T != nullptr) {
            for (int i = 0; i < 3; ++i) {
                newTrans->rotation[i] -= T->rotation[i];
                newTrans->translation[i] -= T->translation[i];
            }
            for (int i = 0; i < 4; ++i) {
                newTrans->scale[i] -= T->scale[i];
            }
        }
    }

    // this function parse attribute atributeName and save it in arr
    static int findAtribute(float* arr, int waitingNumberArg, const char* atributeName, XMLElement* xmlElem) {
        const char* atribute = xmlElem->Attribute(atributeName);
        if (atribute == nullptr)
            return 0;
        char* pEnd;
        arr[0] = (float)strtod(atribute, &pEnd);
        int i = 1;
        float d;
        while (i < waitingNumberArg) {
            d = (float)strtod(pEnd, &pEnd);
            arr[i++] = (float)d;
        }
        return i;
    }

    static int findAtribute2(float* arr, int waitingNumberArg, const char* atributeName, XMLElement* xmlElem) {
        const char* atribute = xmlElem->Attribute(atributeName);
        if (atribute == nullptr)
            return 0;
        char* pEnd;
        arr[0] = (float)strtod(atribute, &pEnd);
        int i = 1;
        float d;
        while (i < waitingNumberArg) {
            d = (float)strtod(pEnd, &pEnd);
            arr[i++] = (float)d;
        }
        return i;
    }
    void ShapeParser(Transformationx3d* t, XMLNode* Shape) {
        auto* newShape = new Shapex3d();
        XMLNode* Node = Shape->FirstChild();
        if (Node == nullptr)
            return;
        if (t != nullptr) {
            newShape->shapeTrans = *t;
        }
        else {
            t = new Transformationx3d();
        }
        XMLNode* ShapeType;
        XMLNode* appearance;
        if ((ShapeType = Shape->FirstChildElement("Sphere")) != nullptr) {
            newShape->shapeType = new Spherex3d(Shape->FirstChildElement("Sphere"));
        }
        if ((ShapeType = Shape->FirstChildElement("Plane")) != nullptr) {
            newShape->shapeType = new Planex3d(Shape->FirstChildElement("Plane"));
        }
        if ((appearance = Shape->FirstChildElement("Appearance")) != nullptr) {
            newShape->shapeAppearance = new Appearancex3d();
            AppearanceParser(newShape->shapeAppearance, appearance);
        }
        list.push(*newShape);
    }

    void AppearanceParser(Appearancex3d* a, XMLNode* Node) {
        if (a == nullptr) return;
        if (Node->FirstChildElement("Material") != nullptr) {
            XMLElement* Elem = Node->FirstChildElement("Material");
            const char* attribute = Elem->Attribute("diffuseColor");
            if (attribute != nullptr && attribute[0] == '#')
                for (int i = 0; i < 3; ++i) {
                    int num = 0;
                    if (attribute[i + 1] >= 'A' && attribute[i + 1] <= 'F')
                        num = (attribute[i + 1] - 'A') * 16;
                    else num = (attribute[i + 1] - '0') * 16;
                    if (attribute[i + 2] >= 'A' && attribute[i + 2] <= 'F')
                        num += (attribute[i + 2] - 'A');
                    else num += (attribute[i + 2] - '0');
                    a->material.diffuseColor[i] = (float)((float)num / 255);
                }
            else if (attribute != nullptr) {
                findAtribute(a->material.diffuseColor, 3, "diffuseColor", Elem);
                attribute = Elem->Attribute("shininess");
                if (attribute != nullptr)
                    findAtribute(&(a->material.shininess), 1, "shininess", Elem);
                attribute = Elem->Attribute("transparency");
                if (attribute != nullptr)
                    findAtribute(&(a->material.transparency), 1, "transparency", Elem);
            }
            if (attribute != nullptr)
                a->material.shininess = (float)atof(attribute);
        }
    }

    void createGetList() {
        while (!list.empty()) {
            Shapex3d Checked_Shape = list.top();
            list.pop();

            auto* scale = new Vector3(Checked_Shape.shapeTrans.scale[0], Checked_Shape.shapeTrans.scale[1],
                Checked_Shape.shapeTrans.scale[2]);
            auto* location = new Vector3(Checked_Shape.shapeTrans.translation[0],
                Checked_Shape.shapeTrans.translation[1],
                Checked_Shape.shapeTrans.translation[2]);
            Object* Obj;
            int red = (int)(Checked_Shape.shapeAppearance->material.diffuseColor[0] * 255);
            int green = (int)(Checked_Shape.shapeAppearance->material.diffuseColor[1] * 255);
            int blue = (int)(Checked_Shape.shapeAppearance->material.diffuseColor[2] * 255);
            auto* rotation = new Quaternion(Checked_Shape.shapeTrans.rotation[3], Checked_Shape.shapeTrans.rotation[0],
                Checked_Shape.shapeTrans.rotation[1], Checked_Shape.shapeTrans.rotation[2]);
            float reflect = Checked_Shape.shapeAppearance->material.transparency;
            float diff = Checked_Shape.shapeAppearance->material.shininess;
            float refract = 1 - reflect - diff;

            Material mat = Material(Vector3(red, green, blue), Vector3(reflect, diff, refract));
            BasicShape* ps;
            if (Checked_Shape.shapeType->typeName == "Sphere") {
                Spherex3d* ps;
                ps = (Spherex3d*)Checked_Shape.shapeType;

                Sphere* Figure = new Sphere(ps->radius, *location, *rotation, *scale, mat);
                listToPop.push(*Figure);
            }
            /*} else if (Checked_Shape.shapeType->typeName == "Plane") {
              Planex3d *ps;
              ps = (Planex3d *) Checked_Shape.shapeType;
              auto *Figure = new Plane(*location, *rotation, *scale, red, green, blue, reflect, diff);
              listToPop.push(*Figure);
            }*/
        }
    }
};
