//
//  ObjMeshLoader2Passes.cpp
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 10/28/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#include "ObjMeshLoader2Passes.h"


bool ObjMeshLoader2Passes::isSplitVertex( const GLuint & index, const GLuint & normalIndex, std::map<GLuint,std::vector<GLuint>*> * normalIndexMap ){
    return false;
}

void ObjMeshLoader2Passes::firstPass(std::ifstream & in)
{
    groups = new std::vector<Group>();
    
    float x, y, z;
    size_t n = 0;
    GLuint * indices;
    glm::vec3 pos;
    glm::vec3 normal;
    Vertex tmp;
    Group * currGroup = NULL;
    
    std::string line;
    while ( std::getline( in, line ) )
    {
        std::size_t pos;
        if ( (pos = line.find( "v " )) != std::string::npos ) // vertices
        {
            sscanf( line.c_str(), "v %f %f %f", &x, &y, &z );
            vertices->push_back( glm::vec3( x, y, z ) );
        }
        else if ( (pos = line.find( "vn " )) != std::string::npos )
        {
            sscanf( line.c_str(), "vn %f %f %f", &x, &y, &z );
            normals->push_back( glm::vec3( x, y, z ) );
        }
        else if ( (pos = line.find( "f " )) != std::string::npos )
        {
            if ( currGroup == NULL )
                currGroup = new Group();
            
            const size_t VERTEX_COUNT = std::count( line.begin(), line.end(), ' ' );
            
            indices = new GLuint[ VERTEX_COUNT ];
            
            Face * face = NULL;
            pos = 0;
            
            for ( int i = 0; i < VERTEX_COUNT; i++)
            {
                // get vertex index
                pos = line.find(" ", pos);
                std::size_t start = pos + 1;
                std::size_t end = line.find_first_of("/", start);
                std::sscanf(line.substr(start, end-start).c_str(), "%zd", &indices[i]);
                
                indices[i] = indices[i] - 1;
                
                // get normal index
                pos = end;
                start = line.find_first_not_of("/", end + 1);
                std::sscanf(line.substr(start).c_str(), "%zd", &n);
                n--;
                
//                for ( std::vector<Face>::iterator iter = currGroup->getFaces()->begin(); iter != currGroup->getFaces()->end(); iter++)
//                {
//                    for ( int j = 0; j < 3; j++)
//                    {
//                        //std::cout << iter->vertexIndices[j] << std::endl;
//                        if (indices[i] == iter->vertexIndices[j] && iter->normalInd != n)
//                        {
//                            glm::vec3 tmp = vertices->at(indices[i]);
//                            glm::vec3 dup;
//                            dup.x = tmp.x; dup.y = tmp.y; dup.z = tmp.z;
//                            vertices->push_back(dup);
//                            indices[i] = (GLuint)vertices->size() - 1;
//                            std::cout << "duplicating verte: " << iter->vertexIndices[j] << ", new index: " << indices[i] << std::endl;
//                        }
//                    }
//                }
                
                currGroup->getIndices()->push_back(indices[i]);
                
                if ( i > 1 )
                {
                    face = new Face();
                    face->vertexIndices[0] = indices[0];
                    face->vertexIndices[1] = indices[i-1];
                    face->vertexIndices[2] = indices[i];
                    
                    face->normalInd = (GLuint)n;
                    face->normal = normals->at(n);
                    currGroup->getFaces()->push_back(*face);
                }
                
//                else if ( i > 2 )
//                {
//                    face = new Face();
//                    face->vertexIndices[0] = indices[0];
//                    face->vertexIndices[1] = indices[i-1];
//                    face->vertexIndices[2] = indices[i];
//                    
//                    face->normal = getNormals()->at(n);
//                    currGroup->getFaces()->push_back(*face);
//                }
            }
        }
        else if ( line.find( "g " ) != std::string::npos || line.find( "o " ) != std::string::npos )
        {
            if ( currGroup )
                groups->push_back( *currGroup );
            currGroup = new Group();
        }
    }
    
    if ( currGroup )
    {
        groups->push_back( *currGroup );
    }
}

void ObjMeshLoader2Passes::secondPass(std::ifstream &in)
{
    Group currGroup;
    size_t * indices;
    size_t n;
    glm::vec3 normal;
    bool vertexNormalCombinationExists = false;
    
    std::set<size_t> sharedVertexNormalPair;
    
    std::string line;
    while ( std::getline( in, line ) )
    {
        std::size_t pos;
        std::size_t groupCount = 0;
        if ( line.find( "g " ) != std::string::npos || line.find( "o " ) != std::string::npos )
        {
            currGroup = groups->at(groupCount++);
        }
        else if ( (pos = line.find( "f " )) != std::string::npos )
        {
            const size_t VERTEX_COUNT = std::count( line.begin(), line.end(), ' ' );
            
            indices = new size_t[ VERTEX_COUNT ];
            pos = 0;
            
            for ( int i = 0; i < VERTEX_COUNT; i++)
            {
                // get vertex index
                pos = line.find(" ", pos);
                std::size_t start = pos + 1;
                std::size_t end = line.find_first_of("/", start);
                std::sscanf(line.substr(start, end-start).c_str(), "%zd", &indices[i]);
//
                indices[i] = indices[i] - 1;
//
//                // get normal index
                pos = end;
                start = line.find_first_not_of("/", end + 1);
                end = line.find(" ", start);
                std::sscanf(line.substr(start, end-start).c_str(), "%zd", &n);
                n--;
                
//                std::pair<std::set<size_t>::iterator,bool> ret;
//                ret = sharedVertexNormalPair.insert(indices[i]);
//                if ( !ret.second )
//                {
//                    for ( std::vector<Face>::iterator iter = currGroup.getFaces()->begin(); iter != currGroup.getFaces()->end(); iter++)
//                    {
//                        glm::vec3 cmpNormal = getNormals()->at(n);
//                        //glm::vec3 iterNormal = iter->normal;
//                        for ( int j = 0; j < VERTEX_COUNT; j++ )
//                            if ( iter->vertexIndices[j] == (GLuint)indices[i] ){
//                                    glm::vec3 duplicate = getVertices()->at( indices[i] );
//                                    glm::vec3 dup;
//                                    dup.x = duplicate.x; dup.y = duplicate.y; dup.z = duplicate.z;
//                                    getVertices()->push_back( dup );
//                                    indices[i] = (GLuint)getVertices()->size() - 1;
//                                    iter->vertexIndices[j] = (GLuint)indices[i];
//                                    iter->normal = cmpNormal;
//                                    iter->normalInd = (GLuint)n;
//                                    currGroup.getIndices()->push_back((GLuint)indices[i]);
////                                    vertexNormalCombinationExists = true;
//                                    break;
//                            }
//                    }
//                }
                
                //int indexCount = 0;
                //std::vector<GLuint> * indicesSwap = new std::vector<GLuint>();
                for ( std::vector<Face>::iterator iter = currGroup.getFaces()->begin(); iter != currGroup.getFaces()->end(); iter++)
                {
                    glm::vec3 cmpNormal = getNormals()->at(n);
                    //glm::vec3 iterNormal = iter->normal;
                    for ( int j = 0; j < VERTEX_COUNT; j++ )
                        if ( iter->vertexIndices[j] == indices[i] ){
                            if ( iter->normal != cmpNormal )
                            {
                                glm::vec3 duplicate = getVertices()->at( indices[i] );
                                glm::vec3 dup;
                                dup.x = duplicate.x; dup.y = duplicate.y; dup.z = duplicate.z;
                                getVertices()->push_back( dup );
                                indices[i] = (GLuint)getVertices()->size() - 1;
                                iter->vertexIndices[j] = indices[i];
                                iter->normal = cmpNormal;
                                //currGroup.getIndices()->push_back(indices[i]);
                                vertexNormalCombinationExists = true;
                                break;
                            }
                        }
                    
                    if ( vertexNormalCombinationExists )
                        break;
                }
//                delete indicesSwap;
            }
            
            delete [] indices;
        }
    }
}

void ObjMeshLoader2Passes::buildNormals()
{
    Group currGroup;
    
    for ( std::vector<Group>::iterator group = groups->begin(); group != groups->end(); group++ )
    {
        for ( std::vector<Face>::iterator face = group->getFaces()->begin(); face != group->getFaces()->end(); face++)
        {
            glm::vec3 p1 = getVertices()->at(face->vertexIndices[0]);
            glm::vec3 p2 = getVertices()->at(face->vertexIndices[1]);
            glm::vec3 p3 = getVertices()->at(face->vertexIndices[2]);
            
            glm::vec3 cross = glm::cross( p3-p1,p1-p1);
            
            getNormals()->push_back(cross);
            face->normalInd = (GLuint)getNormals()->size() - 1;
            face->normal = cross;
        }
    }
}

void ObjMeshLoader2Passes::loadFromFile( const GLchar * path )
{
    std::ifstream in( path );
    
    if ( !in.is_open() )
    {
        std::cout << "Error opening file: " << path << std::endl;
        return;
    }
    
    firstPass(in);
    
    // reset the ifstream
    in.clear();
    in.seekg(0, std::ios::beg);
    
    // do second pass
    //secondPass(in);
    
    in.close();
    
    // rebuild normals
    //buildNormals();
    
    
}

