//
//  TestScene.cpp
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/1/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#include "TestScene.h"
#include "../shapes/Cube.h"
#include "../shapes/Mesh.h"
#include "../shapes/ObjMeshExperiment.h"
#include "../shapes/TestMesh.h"
#include <unistd.h>

void TestScene::prepareScene()
{
    char * buf = new char[256];
    
    //std::cout << "current directory " << getcwd( buf, 256) << std::endl;
    
    //Cube * cube = new Cube();
    //Shape * mesh = new ObjMeshExperiment("Lara_Croft.obj");//new ObjMeshExperiment("Lara_Croft.obj");//new ObjMeshExperiment("monkeySphere.obj");//new ObjMeshExperiment( "monkeyLowPoly.obj" );//new Mesh( "cubeGroupBasic.obj" );//new TestMesh();//new Mesh( "cube.obj" );///new TestMesh();//new Mesh( "simplePlane.obj" );//new Mesh( "simplePlaneGroups.obj" );//new Mesh( "simpleGroup.obj" );//new Mesh( "groupTest.obj" );//new Mesh( "cube.obj" ); //new Mesh( "groupTest.obj" ); new Mesh( "sls_amg_scaled.obj" ); /*new Mesh( "STI_scaled.obj" ); ( "Aventador.obj" ); //( "Porshe_911_GT2.obj" );*/
    //Shape * mesh = new ObjMeshExperiment("data/shark.obj");
    //Shape * mesh2 = new ObjMeshExperiment("AventadorTriangulated.obj");
    //Shape * mesh = new ObjMeshExperiment("UVplane.obj");
    //Shape * mesh3 = new ObjMeshExperiment("Triss.obj");
    //Shape * mesh4 = new ObjMeshExperiment("Porshe_911_GT2.obj");
    Shape * mesh4 = new ObjMeshExperiment("../models/groupTest.obj");
    
    
    //printf("mesh addresses VBO[POSITIONS_BUFF]: %d, VBO[INDEX_BUFF]: %d, VAO[0]: %d\n", mesh->getVBOPointer(Shape::POSITIONS_BUFF), mesh->getVBOPointer(Shape::INDEX_BUFF), mesh->getVAOPointer(0) );
    //printf("mesh2 addresses VBO[POSITIONS_BUFF]: %d, VBO[INDEX_BUFF]: %d, VAO[0]: %d\n", mesh2->getVBOPointer(Shape::POSITIONS_BUFF), mesh2->getVBOPointer(Shape::INDEX_BUFF), mesh2->getVAOPointer(0) );
    
    //objects.push_back( mesh );
    //objects.push_back( mesh2 );
    //objects.push_back(mesh3);
    objects.push_back(mesh4);
}
