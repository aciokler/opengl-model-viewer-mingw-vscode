Third Party Libraries

GLFW: http://www.glfw.org

GLM: https://glm.g-truc.net

GLAD: https://glad.dav1d.de

Compile and run:

Mac OS X:

cd Make

make -f MakeMacOSX

./modelViewer

Linux:

cd Make
make -f MakeLinux

..enjoy


{
package io.pivotal.microservices.accounts;

public class Test {

    public static void main(String [] args) {
        
        float g_vertex_buffer_data[] = { 
                // triangle 1
                -1.0f,-1.0f,-1.0f,
                -1.0f,-1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                // triangle 2
                 1.0f, 1.0f,-1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f,-1.0f,
             // triangle 3
                 1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f,-1.0f,
                 1.0f,-1.0f,-1.0f,
              // triangle 4
                 1.0f, 1.0f,-1.0f,
                 1.0f,-1.0f,-1.0f,
                -1.0f,-1.0f,-1.0f,
             // triangle 5
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f,-1.0f,
             // triangle 6
                 1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f,-1.0f,
             // triangle 7
                -1.0f, 1.0f, 1.0f,
                -1.0f,-1.0f, 1.0f,
                 1.0f,-1.0f, 1.0f,
              // triangle 8
                 1.0f, 1.0f, 1.0f,
                 1.0f,-1.0f,-1.0f,
                 1.0f, 1.0f,-1.0f,
              // triangle 9
                 1.0f,-1.0f,-1.0f,
                 1.0f, 1.0f, 1.0f,
                 1.0f,-1.0f, 1.0f,
              // triangle 10
                 1.0f, 1.0f, 1.0f,
                 1.0f, 1.0f,-1.0f,
                -1.0f, 1.0f,-1.0f,
               // triangle 11
                 1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
             // triangle 12
                 1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                 1.0f,-1.0f, 1.0f
            };
        
        for (int i = 0, p = 1; i+9 <= g_vertex_buffer_data.length; i += 9, p++) {
            int xIndexBase = i;
            float tmpX1 = g_vertex_buffer_data[xIndexBase] - g_vertex_buffer_data[xIndexBase+3];
            float tmpX2 = g_vertex_buffer_data[xIndexBase+6] - g_vertex_buffer_data[xIndexBase+3];
            
            int yIndexBase = i+1;
            float tmpY1 = g_vertex_buffer_data[yIndexBase] - g_vertex_buffer_data[yIndexBase+3];
            float tmpY2 = g_vertex_buffer_data[yIndexBase+6] - g_vertex_buffer_data[yIndexBase+3];
            
            int zIndexBase = i+2;
            float tmpZ1 = g_vertex_buffer_data[zIndexBase] - g_vertex_buffer_data[zIndexBase+3];
            float tmpZ2 = g_vertex_buffer_data[zIndexBase+6] - g_vertex_buffer_data[zIndexBase+3];
            
            double normalX = tmpY1*tmpZ2-tmpZ1*tmpY2;
            double normalY = -1*(tmpX1*tmpZ2-tmpZ1*tmpX2);
            double normalZ = tmpX1*tmpY2-tmpY1*tmpX2;
            
            double normalLength = Math.abs(Math.sqrt(normalX*normalX + normalY*normalY + normalZ*normalZ));
            
            double normalizedX = normalX / normalLength;
            double normalizedY = normalY / normalLength;
            double normalizedZ = normalZ / normalLength;
            
            System.out.println("// triangle " + p);
            System.out.println(g_vertex_buffer_data[xIndexBase] + "f, " + g_vertex_buffer_data[yIndexBase] + "f, " + g_vertex_buffer_data[zIndexBase] + "f,");
            System.out.println(normalizedX + "f, " + normalizedY + "f, " + normalizedZ + "f,");
            System.out.println(g_vertex_buffer_data[xIndexBase+3] + "f, " + g_vertex_buffer_data[yIndexBase+3] + "f, " + g_vertex_buffer_data[zIndexBase+3] + "f,");
            System.out.println(normalizedX + "f, " + normalizedY + "f, " + normalizedZ + "f,");
            System.out.println(g_vertex_buffer_data[xIndexBase+6] + "f, " + g_vertex_buffer_data[yIndexBase+6] + "f, " + g_vertex_buffer_data[zIndexBase+6] + "f,");
            System.out.println(normalizedX + "f, " + normalizedY + "f, " + normalizedZ + "f,");
        }
        
    }
    
     
    
}
}
