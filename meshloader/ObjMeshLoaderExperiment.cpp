//
//  ObjMeshLoaderExperiment.cpp
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 6/29/15.
//  Copyright (c) 2015 Abraham-mac. All rights reserved.
//

#include "ObjMeshLoaderExperiment.h"

bool ObjMeshLoaderExperiment::isSplitVertex( const GLuint & index, const GLuint & normalIndex, std::map<GLuint,std::vector<GLuint>*> * normalIndexMap ){
    return false;
}

void ObjMeshLoaderExperiment::loadFromFile( const GLchar * path )
{
    std::string line;
    std::ifstream in( path );
    
    if ( !in.is_open() )
    {
        std::cout << "Error opening file: " << path << std::endl;
        return;
    }
    
    groups = new std::vector<Group>();
    
    float x, y, z;
    size_t n = 0; //a, b, c, d, e, n;
    GLuint * indices;
    glm::vec3 pos;
    glm::vec3 normal;
    Vertex tmp;
    GLuint indexOffset = 0;
    GLuint groupNr = 0;
    GLuint prevGroupIndexOffset = 0;
    Group * currGroup = NULL;
    
    while ( std::getline( in, line ) )
    {
        std::size_t pos;
        if ( (pos = line.find( "v " )) != std::string::npos ) // vertices
        {
            //printf("v chosen\n");
            sscanf( line.c_str(), "v %f %f %f", &x, &y, &z );
            vertices->push_back( glm::vec3( x, y, z ) );
        }
        else if ( (pos = line.find( "vn " )) != std::string::npos )
        {
            sscanf( line.c_str(), "vn %f %f %f", &x, &y, &z );
            //std::cout << "adding a normal. size (before) " << currGroup->getNormals()->size() << std::endl;
            normals->push_back( glm::vec3( x, y, z ) );
        }
        else if ( (pos = line.find( "f " )) != std::string::npos )
        {
            if ( currGroup == NULL )
                currGroup = new Group();
            
            const size_t VERTEX_COUNT = std::count( line.begin(), line.end(), ' ' );
            
            indices = new GLuint[ VERTEX_COUNT ];
            
            Face * face = NULL;
            
            //printf("LINE: %s\n", line.c_str());
            
            pos = 0;
            for ( int i = 0; i < VERTEX_COUNT; i++)
            {
                // get vertex index
                pos = line.find(" ", pos);
                std::size_t start = pos + 1;//line.find_first_not_of(" ", pos + 1);
                std::size_t end = line.find_first_of("/", start);
                std::sscanf(line.substr(start, end).c_str(), "%zd", &indices[i]);
                
                indices[i] = indices[i] + prevGroupIndexOffset - 1;
                
                // get normal index
                pos = end;
                start = line.find_first_not_of("/", end + 1);
                std::sscanf(line.substr(start).c_str(), "%zd", &n);
                //std::cout << "n" << n << std::endl;
                n--;
                
                //printf("size: %zd\n", currGroup->getFaces()->size());
                bool newVerticesFound = false;
                for ( std::vector<Face>::iterator iter = currGroup->getFaces()->begin(); iter != currGroup->getFaces()->end(); iter++)
                {
                    for ( int j = 0; j < 3; j++)
                    {
                        //std::cout << iter->vertexIndices[j] << std::endl;
                        if (indices[i] == iter->vertexIndices[j] && iter->normalInd != n)
                        {
                            newVerticesFound = true;
                            glm::vec3 tmp = vertices->at(indices[i]);
                            glm::vec3 dup;
                            dup.x = tmp.x; dup.y = tmp.y; dup.z = tmp.z;
                            vertices->push_back(dup);
                            indices[i] = (GLuint)vertices->size() - 1;
                            //std::cout << "duplicating vertex: " << iter->vertexIndices[j] << ", new index: " << indices[i] << std::endl;
                            
                            indexOffset++;
                            currGroup->getIndices()->push_back(indices[i]);
                            break;
                        }
                    }
                }
                
                if ( !newVerticesFound )
                {
                    //indices[i] = indices[i] + prevGroupIndexOffset;
                    currGroup->getIndices()->push_back(indices[i]);
                }
                
                //                if(hasTextureIndex)
                //                {
                //                    pos = start;
                //                }
                
                if ( i > 1 )
                {
                    face = new Face();
                    face->vertexIndices[0] = indices[i-2];
                    face->vertexIndices[1] = indices[i-1];
                    face->vertexIndices[2] = indices[i];
                    
                    //printf("TRIANGLE indices: %zd %zd %zd, normal %zd\n", face->vertexIndices[0], face->vertexIndices[1], face->vertexIndices[2], n);
                    
                    face->normalInd = (GLuint)n;
                    face->normal = normals->at(n);
                    currGroup->getFaces()->push_back(*face);
                }
            }
        }
        else if ( line.find( "g " ) != std::string::npos || line.find( "o " ) != std::string::npos )
        {
            if ( currGroup )
                groups->push_back( *currGroup );
            currGroup = new Group();
            groupNr++;
            prevGroupIndexOffset = indexOffset;
            printf("creating another group: %d\n", groupNr);
        }
    }
    
    in.close();
    
    
    
    if ( currGroup )
    {
        groups->push_back( *currGroup );
    }
}