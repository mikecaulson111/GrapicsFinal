//  Per Pixel Lighting shader from professor modified to be spotlight
#version 120

varying vec3 View;
varying vec3 Light[3];
varying vec3 Normal;
varying vec3 LDir[3];
varying float cutoff[3];

void main()
{

   //  Vertex location in modelview coordinates
   vec4 P = gl_ModelViewMatrix * gl_Vertex;

   //  Light position for each light
   Light[0]  = gl_LightSource[1].position.xyz - P.xyz;
   Light[1] = gl_LightSource[2].position.xyz - P.xyz;
   Light[2] = gl_LightSource[3].position.xyz - P.xyz;
   // Sets light direction for each light
   LDir[0] = gl_LightSource[1].spotDirection.xyz;
   LDir[1] = gl_LightSource[2].spotDirection.xyz;
   LDir[2] = gl_LightSource[3].spotDirection.xyz;

   // Sets cutoff for each light
   cutoff[0] = gl_LightSource[1].spotCutoff;
   cutoff[1] = gl_LightSource[2].spotCutoff;
   cutoff[2] = gl_LightSource[3].spotCutoff;

   // LDir = P.xyz;


   //  Normal
   Normal = gl_NormalMatrix * gl_Normal;

   //  Texture
   gl_TexCoord[0] = gl_MultiTexCoord0;
   //  Eye position
   View  = -P.xyz;
   //  Set vertex position
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}