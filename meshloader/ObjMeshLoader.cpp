//
//  ObjMeshLoader.cpp
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/1/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#include "ObjMeshLoader.h"

bool ObjMeshLoader::isSplitVertex( const GLuint & index, const GLuint & normalIndex, std::map<GLuint,std::vector<GLuint>*> * normalIndexMap ){
    
    
    
    return false;
}

void ObjMeshLoader::loadFromFile( const GLchar * path )
{
    //printf("inside the obj loader\n");
    std::string line;
    std::ifstream in( path );
    
    if ( !in.is_open() )
    {
        std::cout << "Error opening file: " << path << std::endl;
        return;
    }
    
    groups = new std::vector<Group>();
    //    normals = new std::vector<glm::vec3>();
    //    faces = new std::vector<Face>();
    //    vertexOrder = new std::vector<std::vector<GLuint>*>();
    //    startOfGroups = new std::vector<GLuint>();
    
    std::vector<GLuint> * indicesSoFar = new std::vector<GLuint>();
    std::map<GLuint,std::vector<GLuint>*> * normalIndicesMap = new std::map<GLuint, std::vector<GLuint>*>();
    
    
    float x, y, z;
    size_t d, n, n1, n2, n3; //a, b, c, d, e, n;
    GLuint * indices;
    //    bool startGroup = false;
    glm::vec3 pos;
    glm::vec3 normal;
    Vertex tmp;
    Group * currGroup = NULL;
    
    //GLuint vertexIndexOffset = 0;
    GLuint normalIndexOffset = 0;
    GLuint duplicateVerticesOffset = 0;
    
    while ( std::getline( in, line ) )
    {
        //printf("line = %s\n", line.c_str() );
        if ( line.find( "v " ) != std::string::npos ) // vertices
        {
            //printf("v chosen\n");
            sscanf( line.c_str(), "v %f %f %f", &x, &y, &z );
            getVertices()->push_back( glm::vec3( x, y, z ) );
        }
        else if ( line.find( "vn " ) != std::string::npos )
        {
            sscanf( line.c_str(), "vn %f %f %f", &x, &y, &z );
            //std::cout << "adding a normal. size (before) " << currGroup->getNormals()->size() << std::endl;
            getNormals()->push_back( glm::vec3( x, y, z ) );
        }
        else if ( line.find( "f " ) != std::string::npos )
        {
            if ( currGroup == NULL )
                currGroup = new Group();
            
            //printf("f chosen\n");
            const size_t VERTEX_COUNT = std::count( line.begin(), line.end(), ' ' );
            
            //std::cout << " vertex per face " << vertex_count << std::endl;
            isTriangleFaces = VERTEX_COUNT == 3;
            
            indices = new GLuint[ VERTEX_COUNT ];
            
            if ( isTriangleFaces ) // faces
            {
                n = n1 = n2 = n3 = 0;
                //printf("BEFORE n: %zd n1: %zd n2: %zd n3: %zd\n", n, n1, n2, n3);
                if ( line.find( "//" ) != std::string::npos )
                {
                    sscanf( line.c_str(), "f %zd//%zd %zd//%zd %zd//%zd", &indices[0], &n1, &indices[1], &n2, &indices[2], &n3 );
                    
                    //printf( "a %zd, b %zd, c %zd, n %zd\n", indices[0], indices[1], indices[2], n1 );
                }
                else if ( std::count( line.begin(), line.end(), '/') > 0 )
                {
                    sscanf( line.c_str(), "f %zd/%zd/%zd %zd/%zd/%zd %zd/%zd/%zd", &indices[0], &d, &n1, &indices[1], &d, &n2, &indices[2], &d, &n3 );
                    
                    //printf( "a %d, b %d, c %d\n", a, b, c );
                }
                else
                {
                    sscanf( line.c_str(), "f %zd %zd %zd", &indices[0], &indices[1], &indices[2] );
                }
                
                n1--; n2--; n3--;
                Face face;
                for ( int i = 0; i < VERTEX_COUNT; i++ )
                {
                    //printf("before %zd, offset %zd\n", indices[i], vertexIndexOffset);
                    //printf("original index %zd\n", indices[i]);
                    //indices[i] = indices[i] + duplicateVerticesOffset - 1; //(vertexIndexOffset + 1);
                    //printf("index %zd\n", indices[i]);
                    indices[i] = indices[i] - 1;
                    //                    std::cout << "indices thus far ";
                    //                    for ( std::vector<GLuint>::iterator vIter = indicesSoFar->begin(); vIter != indicesSoFar->end(); vIter++ )
                    //                        std::cout << *vIter << ", ";
                    //                    std::cout << std::endl;
                    
                    //TODO fix this!!!!
//                    bool exists = std::find( currGroup->getIndices()->begin(), currGroup->getIndices()->end(), indices[i] ) != indicesSoFar->end();
//                    bool vertexNormalCombinationExists = false;
//                    if ( exists )
//                    {
//                        for ( std::vector<Face>::iterator iter = currGroup->getFaces()->begin(); iter != currGroup->getFaces()->end(); iter++)
//                        {
//                            glm::vec3 cmpNormal = getNormals()->at(n1);
//                            glm::vec3 iterNormal = iter->normal;
//                            if ( iterNormal.x == normal.x && iterNormal.y == normal.y && iterNormal.z == normal.z )
//                                for ( int j = 0; j < VERTEX_COUNT; j++ )
//                                    if ( iter->vertexIndices[j] == indices[i] ){
//                                        vertexNormalCombinationExists = true;
//                                        break;
//                                    }
//                            
//                            if ( vertexNormalCombinationExists )
//                                break;
//                        }
//                        
//                        if ( !vertexNormalCombinationExists )
//                        {
//                            //printf("there exists\n");
//                            //std::cout << "size of vertices before adding duplicate " << currGroup->getVertices()->size() << std::endl;
//                            std::cout << "found duplicate of " << indices[i] << std::endl;
//                            glm::vec3 duplicate = getVertices()->at( indices[i] );
//                            glm::vec3 dup;
//                            dup.x = duplicate.x; dup.y = duplicate.y; dup.z = duplicate.z;
//                            getVertices()->push_back( dup );
//                            indices[i] = (GLuint)getVertices()->size() - 1;
//                            //duplicateVerticesOffset++;
//                        }
//                    }
                    indicesSoFar->push_back( indices[i] );
                    
                    face.vertexIndices[i] = indices[i];
                    
                    currGroup->getIndices()->push_back( indices[i] );
                }
                
                // -= normalIndexOffset + 1;
                //normalIndicesMap-> =
                //std::cout << "n after decrement " << n << std::endl;
                
                //std::cout << "size of normals " << getNormals()->size() << ", normal index " << n << std::endl;
                //printf("normal %f %f %f\n", getNormals()->at(n).x, getNormals()->at(n).y, getNormals()->at(n).z );
                //std::cout << "normal " << getNormals()->at(n).x << ", " << getNormals()->at(n).y << ", " << getNormals()->at(n).z << std::endl;
                //printf("AFTER n: %zd n1: %zd n2: %zd n3: %zd\n", n, n1, n2, n3);
                face.normal = getNormals()->at(n1);
                //printf("normal from face %f %f %f\n", face.normal.x, face.normal.y, face.normal .z );
                currGroup->getFaces()->push_back(face);
                
                
                
                
                //                if ( startGroup )
                //                {
                //                    startGroup = false;
                //                    startOfGroups->push_back( indices[0] );
                //                }
                
                //printf( "mesh v %d %d %d\n", face.vertexIndices[0], face.vertexIndices[1], face.vertexIndices[2] );
                //printf( "mesh vn %f %f %f\n", face.normal.x, face.normal.y, face.normal.z );
            }
            else
            {
                for ( int i = 0; i < VERTEX_COUNT; i++ )
                {
                    
                }
            }
        }
        else if ( line.find( "g " ) != std::string::npos || line.find( "o " ) != std::string::npos )
        {
            
            //            startGroup = true;
            if ( currGroup )
            {
                //printf( "before inserting group\n" );
                groups->push_back( *currGroup );
                
                //vertexIndexOffset += currGroup->getVertices()->size() - duplicateVertices;
                //normalIndexOffset += currGroup->getNormals()->size();
                //duplicateVertices = 0;
            }
            
            //printf( "creating group\n" );
            currGroup = new Group();
            
            //std::cout << " size of normals after group creation " << currGroup->getNormals()->size() << std::endl;
            
            // remove temporary index cache
            //indicesSoFar->clear();
        }
    }
    
    
    
    if ( currGroup )
    {
        //printf( "before inserting group\n" );
        groups->push_back( *currGroup );
    }
    
    
}