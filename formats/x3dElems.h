#import "Parser.h"
#import "Parser.cpp"
#include "tinyxml2.h"
#include <vector>
#include <map>
#include <stack>
#include <string>

using tinyxml2::XMLDocument;
using tinyxml2::XMLError;
using tinyxml2::XMLNode;
using tinyxml2::XMLElement;
using tinyxml2::XML_SUCCESS;

static int findAtribute2(float *arr, int waitingNumberArg, const char *atributeName, XMLElement *xmlElem) {
  const char *atribute = xmlElem->Attribute(atributeName);
  if (atribute == nullptr)
    return 0;
  char *pEnd;
  arr[0] = (float) strtod(atribute, &pEnd);
  int i = 1;
  float d;
  while (i < waitingNumberArg) {
    d = (float) strtod(pEnd, &pEnd);
    arr[i++] = (float) d;
  }
  return i;
}

class Materialx3d {
  public:
  float shininess;
  float diffuseColor[3] = {0.8, 0.8, 0.8};
  float transparency;

  //float ambientIntensity;//  not use
  //float emissiveColor[3];// now not use
  //float specularColor[3];// not use
  //float transparency;// not use
  Materialx3d() {
    for (float &i : diffuseColor) {
      i = 0.8;
    }
    shininess = 0.2;
    transparency = 0;
  }
};

class Appearancex3d {
  public:
  Materialx3d material;
};

class Transformationx3d {
  public:
  float rotation[4]{};
  float scale[3]{};
  float translation[3]{};

  Transformationx3d() {
    for (int i = 0; i < 3; ++i) {
      rotation[i] = 0;
      scale[i] = 0;
      translation[i] = 0;
    }
    rotation[3] = 0;
    rotation[2] = 1;
  }
};

class type {
  public:
  std::string typeName;

  type() {
    typeName = "I don't know who is me!";
  }
};

class Shapex3d {
  public:
  Appearancex3d *shapeAppearance;
  Transformationx3d shapeTrans;
  type *shapeType;

  Shapex3d() {
    shapeAppearance = new Appearancex3d();
    shapeType = new type();
  }
};

class Spherex3d : public type {
  public:
  float radius;

  explicit Spherex3d(XMLNode *Node) {
    typeName = "Sphere";
    radius = 2;
    if (Node != nullptr) {
      XMLElement *elem;
      if (!(elem = Node->ToElement()))
        if (!elem->Attribute("radius"))
          radius = atof(elem->Attribute("radius"));
    }
  }

  Spherex3d() {
    typeName = "Sphere";
    radius = 2;
  }
};

class Boxx3d : public type {
  public:
  float x, y, z;

  Boxx3d() {
    typeName = "Box";
    x = y = z = 2;
  }
};

class Conex3d : public type {
  public:
  float bottomRadius;
  float height;
  float topRadius;

  Conex3d() {
    typeName = "Cone";
    bottomRadius = 1.0;
    height = 2.0;
    topRadius = 0.0;
  }
};

class Cylinderx3d : public type {
  public:
  float radius;
  float height;

  Cylinderx3d() {
    typeName = "Cylinder";
    height = 2.0;
    radius = 1.0;
  }
};

class Planex3d : public type {
  public:
  float center[3]{};
  float size[3]{};

  explicit Planex3d(XMLNode *Node) {
    typeName = "Plane";
    if (Node && Node->ToElement()) {
      XMLElement *elem = Node->ToElement();
      if (findAtribute2(center, 3, "center", elem) < 3)
        center[0] = center[1] = center[2] = 0;
      if (findAtribute2(size, 2, "size", elem) < 2)
        size[0] = size[1] = 2;
      size[2] = 0;
    } else {
      center[0] = center[1] = center[2] = 0;
      size[0] = size[1] = size[2] = 1;
    }
  }
};
