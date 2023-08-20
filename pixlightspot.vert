// FLASHLIGHT using per pixel lighting, modified from professor's code

//  Per Pixel Lighting shader
#version 120

varying vec3 View;
varying vec3 Light;
varying vec3 Normal;
varying vec3 LDir;

void main()
{

   //  Vertex location in modelview coordinates
   vec4 P = gl_ModelViewMatrix * gl_Vertex;

   //  Light position
   Light  = -P.xyz;
   // Sets light direction
   LDir = gl_LightSource[6].spotDirection.xyz;
   //  Normal
   Normal = gl_NormalMatrix * gl_Normal;

   //  Texture
   gl_TexCoord[0] = gl_MultiTexCoord0;
   //  Eye position
   View  = -P.xyz;
   //  Set vertex position
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}