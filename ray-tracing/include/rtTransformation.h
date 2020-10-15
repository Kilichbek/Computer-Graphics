#pragma once

#include "ofMain.h"
#include "rtLinearAlgebra.h"

// https://github.com/sraaphorst/raytracer/blob/master/src/affine_transform.h

static glm::mat4 translation(float x,float y,float z)
{
    glm::vec3 point(x,y,z);
    glm::mat4 m;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(i==j) m[i][j]=1;
            else if (i==3) m[i][j] = point[j];
            else m[i][j] = 0;
        }
    }
    return m;
}

static glm::mat4 scale(float x, float y, float z)
{
    glm::mat4 m;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            m[i][j] = 0;
        }
    }

    m[0][0] = x;
    m[1][1] = y;
    m[2][2] = z;
    m[3][3] = 1;
    
    return m;
}


static glm::mat4 rotation_x(float theta)
{
    glm::mat4 m;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if ((i == 1 && j == 1) || (i == 2 && j == 2))
                m[i][j] = cos(theta);
            else if (i == 1 && j == 2)
                m[j][i] = -sin(theta);
            else if (i == 2 && j == 1)
                m[j][i] = sin(theta);
            else if (i == j)
                m[i][j] = 1;
            else m[i][j] = 0;
    return m;
}


static glm::mat4 rotation_y(float theta)
{
    glm::mat4 m;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if ((i == 0 && j == 0) || (i == 2 && j == 2))
                m[i][j] = cos(theta);
            else if (i == 0 && j == 2)
                m[j][i] = sin(theta);
            else if (i == 2 && j == 0)
                m[j][i] = -sin(theta);
            else if (i == j)
                m[i][j] = 1;
            else m[i][j] = 0;
    return m;
}


static glm::mat4 rotation_z(float theta)
{
    glm::mat4 m;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if ((i == 0 && j == 0) || (i == 1 && j == 1))
                m[i][j] = cos(theta);
            else if (i == 0 && j == 1)
                m[j][i] = -sin(theta);
            else if (i == 1 && j == 0)
                m[j][i] = sin(theta);
            else if (i == j)
                m[i][j] = 1;
            else m[i][j] = 0;
    return m;
}