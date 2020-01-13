//
//  ObjMeshLoaderQuadsExperiment.cpp
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 6/29/16.
//  Copyright © 2016 Abraham-mac. All rights reserved.
//

#include "ObjMeshLoaderQuadsExperiment.h"

bool ObjMeshLoaderQuadsExperiment::isSplitVertex( const GLuint & index, const GLuint & normalIndex, std::map<GLuint,std::vector<GLuint>*> * normalIndexMap ){
    return false;
}

void ObjMeshLoaderQuadsExperiment::loadFromFile( const GLchar * path )
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
    size_t n = 0, t = 0; //a, b, c, d, e, n;
    //size_t faceMaxVertexCount = 3;
    GLint index = 0;
    GLuint * indices = NULL;
    GLuint * normalIndices = NULL;
    std::size_t currentIndicesSize = 0;
    std::size_t originalVerticesSize = 0;
    std::size_t originalNormalsSize = 0;
    glm::vec3 pos;
    glm::vec3 normal;
    Vertex tmp;
    GLuint indexOffset = 0;
    GLuint groupNr = 0;
    GLuint prevGroupIndexOffset = 0;
    Group * currGroup = NULL;
    std::map<GLuint, Face*> groupIndexFacesMap;
    GLuint isSmoothingGroup = false;
    GLuint lineCounter = 0;
    
    while ( std::getline( in, line ) )
    {
        //std::cout << "line: " << line << std::endl;
        lineCounter++;
        std::size_t pos;
        if ( (pos = line.find( "v " )) != std::string::npos ) // vertices
        {
            sscanf( line.c_str(), "v %f %f %f", &x, &y, &z );
            vertices->push_back( glm::vec3( x, y, z ) );
            originalVerticesSize++;
        }
        else if ( (pos = line.find( "vn " )) != std::string::npos )
        {
            sscanf( line.c_str(), "vn %f %f %f", &x, &y, &z );
            normals->push_back( glm::vec3( x, y, z ) );
            originalNormalsSize++;
        }
        else if ( (pos = line.find( "s " )) != std::string::npos && pos == 0 )
        {
            //std::cout << "S SECTION line#: " << lineCounter << std::endl;
            //std::cout << "found smothing flag" << std::endl;
            pos = line.find(" ", pos);
            if ( line.find(" 0", pos) == std::string::npos && line.find("off", pos) == std::string::npos )
            {
                //std::cout << "its true" << std::endl;
                isSmoothingGroup = true;
            }
            else
            {
                //std::cout << "it's 0 or OFF" << std::endl;
                isSmoothingGroup = false;
            }
            
        }
        else if ( (pos = line.find( "f " )) != std::string::npos )
        {
            //std::cout << "F SECTION line#: " << lineCounter << std::endl;
            if ( currGroup == NULL )
                currGroup = new Group();
            
            size_t VERTEX_COUNT = std::count( line.begin(), line.end(), ' ' );
            //std::cout << "vertex count: " << VERTEX_COUNT << std::endl;
            if ( line.at(line.length()-1) == ' ' || line.at(line.length()-2) == ' ')
            {
                VERTEX_COUNT--;
            }
            
            if ( currentIndicesSize < VERTEX_COUNT )
            {
                delete [] indices;
                delete [] normalIndices;
                
                // initialize new indices arrays...
                indices = new GLuint[ VERTEX_COUNT ];
                normalIndices = new GLuint[ VERTEX_COUNT ];
                
                currentIndicesSize = VERTEX_COUNT;
            }
            else
            {
                for ( int i = 0; i < currentIndicesSize; i++ )
                    indices[i] = normalIndices[i] = -1;
            }
                
            Face * face = NULL;
            
            glm::vec3 smoothedNormal;
            pos = 0;
            for ( int i = 0; i < VERTEX_COUNT; i++)
            {
                pos = line.find(" ", pos);
                std::size_t start = pos + 1;
                std::size_t end = line.find_first_of("/", start);
                
                // check if the indices are relative (negative values) or absolute
                if (line.at(start) == '-')
                {
                    //std::cout << "found negative index" << std::endl;
                    std::sscanf(line.substr(start+1, end).c_str(), "%zd", &indices[i]);
                    indices[i] = (GLuint)(originalVerticesSize - (indices[i] - 1));
                }
                else
                {
                    //std::cout << "found positive index" << std::endl;
                    std::sscanf(line.substr(start, end).c_str(), "%zd", &indices[i]);
                }
                
                //std::cout << "read index from file: " << indices[i] << std::endl;
                
                indices[i] = indices[i] + prevGroupIndexOffset - 1;
                
                // skip over texture index for now...
                start = end + 1;
                end = line.find_first_of("/", start);
                
                // get normal index
                pos = end;
                start = line.find_first_not_of("/", end + 1);
                if ( line.at(start) == '-' )
                {
                    std::sscanf(line.substr(start+1).c_str(), "%zd", &n);
                    n = originalNormalsSize - (n-1);
                }
                else
                    std::sscanf(line.substr(start).c_str(), "%zd", &n);
                
                //std::cout << "normal " << n << std::endl;
                n--;
                normalIndices[i] = (GLuint)n;
//            }
//            
//            for ( int i = 0; i < VERTEX_COUNT; i++)
//            {
                //std::cout << "smoothing group: " << isSmoothingGroup << std::endl;
                if ( !isSmoothingGroup )
                {
                    bool newVerticesFound = false;
                    bool isSmoothedNormal = false;
                    std::map<GLuint, Face*>::iterator faceItr = groupIndexFacesMap.find(indices[i]);
                    if ( faceItr != groupIndexFacesMap.end() )
                    {
                        int j = 0;
                        for ( j = 0; j < 3; j++ )
                            if ( faceItr->second->vertexIndices[j] == indices[i] )
                                break;
                        
                        //std::cout << "found face" << std::endl;
                        if ( faceItr->second->vertexNormalIndices[j] != n )
                        {
                            newVerticesFound = true;
                            //std::cout << "index(" << i <<"): " << indices[i] << ", size of vertices: " << vertices->size() << std::endl;
                            glm::vec3 tmp = vertices->at(indices[i]);
                            glm::vec3 dup;
                            dup.x = tmp.x; dup.y = tmp.y; dup.z = tmp.z;
                            vertices->push_back(dup);
                            indices[i] = (GLuint)vertices->size() - 1;
                            //std::cout << "duplicating vertex: " << faceItr->second->vertexIndices[j] << ", new index: " << indices[i] << std::endl;
                            //
                            indexOffset++;
                        }
                    }
                }
                
                currGroup->getIndices()->push_back(indices[i]);
                
                if ( i > 1 )
                {
                    
//                    if ( i > 2 )
//                    {
//                        face = new Face();
//                        
//                        face->vertexIndices[0] = indices[i-2];
//                        face->vertexIndices[1] = indices[i-1];
//                        face->vertexIndices[2] = indices[i];
//                        
//                        // adding face to map for fast searching times...
//                        groupIndexFacesMap[indices[i-2]] = face;
//                        groupIndexFacesMap[indices[i-1]] = face;
//                        groupIndexFacesMap[indices[i]] = face;
//                        
//                        //printf("TRIANGLE indices: %zd %zd %zd, normals: %zd %zd %zd\n", face->vertexIndices[0], face->vertexIndices[1], face->vertexIndices[2], normalIndices[0], normalIndices[i-1],normalIndices[i]);
//                        
//                        face->vertexNormalIndices[0] = normalIndices[i-2];
//                        face->vertexNormalIndices[1] = normalIndices[i-1];
//                        face->vertexNormalIndices[2] = normalIndices[i];
//                        
//                        currGroup->getFaces()->push_back(*face);
//                        
//                        delete face;
//                        face = new Face();
//                        
//                        face->vertexIndices[0] = indices[i-3];
//                        face->vertexIndices[1] = indices[i-2];
//                        face->vertexIndices[2] = indices[i];
//                        
//                        // adding face to map for fast searching times...
//                        groupIndexFacesMap[indices[i-3]] = face;
//                        groupIndexFacesMap[indices[i-2]] = face;
//                        groupIndexFacesMap[indices[i]] = face;
//                        
//                        //printf("TRIANGLE indices: %zd %zd %zd, normals: %zd %zd %zd\n", face->vertexIndices[0], face->vertexIndices[1], face->vertexIndices[2], normalIndices[0], normalIndices[i-1],normalIndices[i]);
//                        
//                        face->vertexNormalIndices[0] = normalIndices[i-3];
//                        face->vertexNormalIndices[1] = normalIndices[i-2];
//                        face->vertexNormalIndices[2] = normalIndices[i];
//                        
//                        currGroup->getFaces()->push_back(*face);
//                    }
//                    else
//                    {
                        face = new Face();
                    
                        //std::cout << "i1 " << indices[i-2] << ", i2 " << indices[i-1] << ", i3 " << indices[i] << std::endl;
                    
                        face->vertexIndices[0] = indices[i-2];
                        face->vertexIndices[1] = indices[i-1];
                        face->vertexIndices[2] = indices[i];
                        
                        // adding face to map for fast searching times...
                        groupIndexFacesMap[indices[i-2]] = face;
                        groupIndexFacesMap[indices[i-1]] = face;
                        groupIndexFacesMap[indices[i]] = face;
                        
                        //printf("TRIANGLE indices: %zd %zd %zd, normals: %zd %zd %zd\n", face->vertexIndices[0], face->vertexIndices[1], face->vertexIndices[2], normalIndices[0], normalIndices[i-1],normalIndices[i]);
                        
                        face->vertexNormalIndices[0] = normalIndices[i-2];
                        face->vertexNormalIndices[1] = normalIndices[i-1];
                        face->vertexNormalIndices[2] = normalIndices[i];
                    
                        currGroup->getFaces()->push_back(*face);
                        delete face;
//                    }
                }
            }
        }
        else if ( line.find( "g " ) != std::string::npos || line.find( "o " ) != std::string::npos )
        {
            if ( currGroup )
            {
                groups->push_back( *currGroup );
                
                isSmoothingGroup = false;
            }
            
            currGroup = new Group();
            groupIndexFacesMap.clear();
            groupNr++;
            prevGroupIndexOffset = indexOffset;
            printf("creating another group: %d\n", groupNr);
        }
    }
    
    in.close();
    groupIndexFacesMap.clear();
    
    delete [] indices;
    delete [] normalIndices;
    
    if ( currGroup )
    {
        groups->push_back( *currGroup );
    }
}