//
//  ObjMeshLoaderLatestFixingBugs.cpp
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/7/16.
//  Copyright © 2016 Abraham-mac. All rights reserved.
//

#include "ObjMeshLoaderLatestFixingBugs.h"

const GLdouble ONEEIGHTY_OVER_PI = 57.2957795131;
const GLfloat DEGREE_MARGIN = 25.00;

bool ObjMeshLoaderLatestFixingBugs::isSplitVertex( const GLuint & index, const GLuint & normalIndex, std::map<GLuint,std::vector<GLuint>*> * normalIndexMap ){
    return false;
}

void ObjMeshLoaderLatestFixingBugs::loadFromFile( const GLchar * path )
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
    
    while ( std::getline( in, line ) )
    {
        std::size_t pos;
        if ( (pos = line.find( "v " )) != std::string::npos ) // vertices
        {
            //printf("v chosen\n");
            sscanf( line.c_str(), "v %f %f %f", &x, &y, &z );
            vertices->push_back( glm::vec3( x, y, z ) );
            originalVerticesSize++;
        }
        else if ( (pos = line.find( "vn " )) != std::string::npos )
        {
            sscanf( line.c_str(), "vn %f %f %f", &x, &y, &z );
            //std::cout << "adding a normal. size (before) " << currGroup->getNormals()->size() << std::endl;
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
            if ( currGroup == NULL )
                currGroup = new Group();
            
            size_t VERTEX_COUNT = std::count( line.begin(), line.end(), ' ' );
            //std::cout << "vertex count: " << VERTEX_COUNT << std::endl;
            //std::cout << "char at the end:-" << line.at(line.length()-1) << "-" << line.at(line.length()-2) << "-" << std::endl;
            if ( line.at(line.length()-1) == ' ' || line.at(line.length()-2) == ' ')
            {
                VERTEX_COUNT--;
            }
            
            //            if ( VERTEX_COUNT > faceMaxVertexCount )
            //            {
            //                if ( indices )
            //                    delete [] indices;
            //                if ( normalIndices )
            //                    delete [] normalIndices;
            //
            //                indices = new GLuint[ VERTEX_COUNT ];
            //                normalIndices = new GLuint[ VERTEX_COUNT ];
            //
            //                faceMaxVertexCount = VERTEX_COUNT;
            //            }
            //            else if ( !indices && !normalIndices )
            //            {
            //                indices = new GLuint[ faceMaxVertexCount ];
            //                normalIndices = new GLuint[ faceMaxVertexCount ];
            //            }
            
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
            
            //printf("LINE: %s\n", line.c_str());
            
            glm::vec3 smoothedNormal;
            pos = 0;
            for ( int i = 0; i < VERTEX_COUNT; i++)
            {
                // get vertex index
                pos = line.find(" ", pos);
                std::size_t start = pos + 1;//line.find_first_not_of(" ", pos + 1);
                std::size_t end = line.find_first_of("/", start);
                //std::sscanf(line.substr(start, end).c_str(), "%zd", &indices[i]);
                
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
                
                indices[i] = indices[i] + prevGroupIndexOffset - 1;
                
                // skip over texture index for now...
                start = end + 1;
                end = line.find_first_of("/", start);
                
                // get normal index
                pos = end;
                start = line.find_first_not_of("/", end + 1);
                //std::sscanf(line.substr(start).c_str(), "%zd", &n);
                if ( line.at(start) == '-' )
                {
                    std::sscanf(line.substr(start+1).c_str(), "%zd", &n);
                    n = originalNormalsSize - (n-1);
                }
                else
                    std::sscanf(line.substr(start).c_str(), "%zd", &n);
                
                //std::cout << "before change: indices[" << i << "]= " << indices[i] << ", n=" << n << std::endl;
                n--;
                normalIndices[i] = (GLuint)n;
                
                if ( !isSmoothingGroup )
                {
                    // printf("is not smoothing group\n");
                    bool newVerticesFound = false;
                    bool isSmoothedNormal = false;
                    std::map<GLuint, Face*>::iterator faceItr = groupIndexFacesMap.find(indices[i]);
                    if ( faceItr != groupIndexFacesMap.end() )
                    {
                        //printf("found sharing face\n");
                        int j = 0;
                        for ( j = 0; j < 3; j++ )
                            if ( faceItr->second->vertexIndices[j] == indices[i] )
                                break;
                        
                        //std::cout << "found face" << std::endl;
                        if ( faceItr->second->vertexNormalIndices[j] != n )
                        {
                            //
                            //                        glm::vec3 curNormal = faceItr->second->normal;
                            //                        glm::vec3 newNormal = normals->at(n);
                            //
                            //                        // determine if angle between faces is greater than smooth margin...
                            //                        GLfloat costheta = glm::dot(curNormal, newNormal) / glm::length(curNormal);
                            //                        GLdouble radians = std::acos(costheta);
                            //                        GLdouble degrees =radians * ONEEIGHTY_OVER_PI;
                            
                            //std::cout << "degree " << degrees << std::endl;
                            //                        if ( std::abs(degrees) > DEGREE_MARGIN )
                            //                        {
                            //
                            newVerticesFound = true;
                            glm::vec3 tmp = vertices->at(indices[i]);
                            glm::vec3 dup;
                            dup.x = tmp.x; dup.y = tmp.y; dup.z = tmp.z;
                            vertices->push_back(dup);
                            indices[i] = (GLuint)vertices->size() - 1;
                            //std::cout << "duplicating vertex: " << faceItr->second->vertexIndices[j] << ", new index: " << indices[i] << std::endl;
                            //
                            indexOffset++;
                            //                        }
                            //                        else if ( i > 1 )
                            //                        {
                            //                            //glm::vec3 newNormal = glm::cross(vertices->at(indices[i-1])-vertices->at(i), vertices->at(indices[i-2])-vertices->at(i));
                            //                            GLfloat area = glm::length(newNormal);
                            //                            curNormal.x = ((curNormal.x) + (newNormal.x * area * radians));
                            //                            curNormal.y = ((curNormal.y) + (newNormal.y * area * radians));
                            //                            curNormal.z = ((curNormal.z) + (newNormal.z * area * radians));
                            //                            faceItr->second->normalWeight++;
                            //                            faceItr->second->normal = curNormal;
                            //                            //normals-> // TODO update normals some how....
                            //                            smoothedNormal = curNormal;
                            //                            isSmoothedNormal = true;
                            //                        }
                        }
                    }
                }
                
                //std::cout << "final change: indices[" << i << "]= " << indices[i] << ", n=" << n << std::endl;
                
                //if ( !newVerticesFound )
                currGroup->getIndices()->push_back(indices[i]);
                
                if ( i > 1 )
                {
                    face = new Face();
                    //                    if ( i > 2)
                    //                    {
                    //                        face->vertexIndices[0] = indices[0];
                    //                        face->vertexIndices[1] = indices[i-1];
                    //                        face->vertexIndices[2] = indices[i];
                    //
                    //                        // adding face to map for fast searching times...
                    //                        groupIndexFacesMap[indices[0]] = face;
                    //                        groupIndexFacesMap[indices[i-1]] = face;
                    //                        groupIndexFacesMap[indices[i]] = face;
                    //
                    //                        //printf("TRIANGLE indices: %zd %zd %zd, normal %zd\n", face->vertexIndices[0], face->vertexIndices[1], face->vertexIndices[2], n);
                    //
                    //                        face->normalInd = (GLuint)n;
                    //
                    //                        face->vertexNormalIndices[0] = normalIndices[0];
                    //                        face->vertexNormalIndices[1] = normalIndices[i-1];
                    //                        face->vertexNormalIndices[2] = normalIndices[i];
                    //                    }
                    //                    else
                    //                    {
                    face->vertexIndices[0] = indices[i];
                    face->vertexIndices[1] = indices[i-1];
                    face->vertexIndices[2] = indices[i-2];
                    
                    // adding face to map for fast searching times...
                    groupIndexFacesMap[indices[i]] = face;
                    groupIndexFacesMap[indices[i-1]] = face;
                    groupIndexFacesMap[indices[i-2]] = face;
                    
                    //printf("TRIANGLE indices: %zd %zd %zd, normals: %zd %zd %zd\n", face->vertexIndices[0], face->vertexIndices[1], face->vertexIndices[2], normalIndices[i], normalIndices[i-1],normalIndices[i-2]);
                    
                    //face->normalInd = (GLuint)n;
                    
                    face->vertexNormalIndices[0] = normalIndices[i];
                    face->vertexNormalIndices[1] = normalIndices[i-1];
                    face->vertexNormalIndices[2] = normalIndices[i-2];
                    //                    }
                    //if ( faceItr != groupIndexFacesMap.end() )
                    //face->normal = faceItr->second->normal;
                    //else
                    //                    if ( isSmoothedNormal )
                    //                    {
                    //                        //face->normal = smoothedNormal;
                    //                    }
                    //                    else
                    //                    {
                    //                        if ( normalIndices[0] == normalIndices[1] == normalIndices[2] )
                    //                            face->normal = normals->at(n);
                    //                        else
                    //                        {
                    //face->normal = glm::normalize(glm::cross(vertices->at(indices[i-2])-vertices->at(indices[i]), vertices->at(indices[i-1])-vertices->at(indices[i])));
                    //face->normal = glm::normalize(normals->at(normalIndices[0]) + normals->at(normalIndices[0]) + normals->at(normalIndices[0]));
                    //                        }
                    //                    }
                    currGroup->getFaces()->push_back(*face);
                }
            }
        }
        else if ( line.find( "g " ) != std::string::npos || line.find( "o " ) != std::string::npos )
        {
            if ( currGroup )
            {
                groups->push_back( *currGroup );
                
                //                for ( std::vector<Face>::iterator iter = currGroup->getFaces()->begin(); iter != currGroup->getFaces()->end(); iter++)
                //                {
                //                    //std::cout << "iterating .... " << iter->vertexIndices[0] << ", " << iter->vertexIndices[1] << ", " << iter->vertexIndices[2] << std::endl;
                //                    glm::vec3 normal = iter->normal;
                ////                    normal.x = normal.x / iter->normalWeight;
                ////                    normal.y = normal.y / iter->normalWeight;
                ////                    normal.z = normal.z / iter->normalWeight;
                //                    normal = glm::normalize(normal);
                //                    iter->normal = normal;
                //                }
                isSmoothingGroup = false;
            }
            
            currGroup = new Group();
            groupIndexFacesMap.clear();
            groupNr++;
            prevGroupIndexOffset = indexOffset;
            //printf("creating another group: %d\n", groupNr);
        }
    }
    
    in.close();
    groupIndexFacesMap.clear();
    
    delete [] indices;
    delete [] normalIndices;
    
    if ( currGroup )
    {
        groups->push_back( *currGroup );
        
        //        for ( std::vector<Face>::iterator iter = currGroup->getFaces()->begin(); iter != currGroup->getFaces()->end(); iter++)
        //        {
        //glm::vec3 normal = iter->normal;
        //normal.x = normal.x / iter->normalWeight;
        //normal.y = normal.y / iter->normalWeight;
        //normal.z = normal.z / iter->normalWeight;
        
        //normal = glm::normalize(normal);
        //iter->normal = normal;
        //        }
    }
}