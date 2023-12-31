//  Cartoon vertex shader
//  Adapted from Lighthouse3D
#version 120

varying vec3 N;
varying vec3 L;
varying vec3 L2;
	
void main()
{
   //  P is the vertex coordinate on body
   vec3 P = vec3(gl_ModelViewMatrix * gl_Vertex);
   //  L is the light vector
   L = vec3(gl_LightSource[0].position) - P;
   // L2 = vec3(gl_LightSource[1].position) - P;
   //  Normal
   N = gl_NormalMatrix * gl_Normal;
   //  Transformed vertex
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
