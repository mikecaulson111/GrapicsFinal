// //  Per Pixel Lighting shader
// #version 120

// varying vec3 View;
// varying vec3 Light;
// varying vec3 Light2;
// varying vec3 Normal;

// void main()
// {
//    //  N is the object normal
//    vec3 N = normalize(Normal);
//    //  L is the light vector
//    vec3 L = normalize(Light);
//    //  R is the reflected light vector R = 2(L.N)N - L
//    vec3 R = reflect(-L,N);
//    //  V is the view vector (eye vector)
//    vec3 V = normalize(View);

//    //  Diffuse light is cosine of light and normal vectors
//    float Id = max(dot(L,N) , 0.0);
//    //  Specular is cosine of reflected and view vectors
//    float Is = (Id>0.0) ? pow(max(dot(R,V),0.0) , gl_FrontMaterial.shininess) : 0.0;

//    //  Sum color types
//    // gl_FragColor = gl_FrontMaterial.emission
//    //              + gl_FrontLightProduct[0].ambient
//    //              + Id*gl_FrontLightProduct[0].diffuse
//    //              + Is*gl_FrontLightProduct[0].specular;


//    // For Second Light, this is just testing.
//    // //  N is the object normal
//    // vec3 N = normalize(Normal);
//    //  L is the light vector
//    vec3 L1 = normalize(Light2);
//    //  R is the reflected light vector R = 2(L.N)N - L
//    vec3 R1 = reflect(-L1,N);
//    // //  V is the view vector (eye vector)
//    // vec3 V1 = normalize(View);

//    //  Diffuse light is cosine of light and normal vectors
//    float Id1 = max(dot(L1,N) , 0.0);
//    //  Specular is cosine of reflected and view vectors
//    float Is1 = (Id1>0.0) ? pow(max(dot(R1,V),0.0) , gl_FrontMaterial.shininess) : 0.0;

//    //  Sum color types
//    gl_FragColor = gl_FrontMaterial.emission
//                 + gl_FrontLightProduct[0].ambient
//                 + Id*gl_FrontLightProduct[0].diffuse
//                 + Is*gl_FrontLightProduct[0].specular
//                 + gl_FrontLightProduct[1].ambient
//                 + Id1*gl_FrontLightProduct[1].diffuse
//                 + Is1*gl_FrontLightProduct[1].specular;
// }



//  Per Pixel Lighting shader
#version 120

varying vec3 View;
varying vec3 Light;
varying vec3 Normal;

void main()
{
   //  N is the object normal
   vec3 N = normalize(Normal);
   //  L is the light vector
   vec3 L = normalize(Light);
   //  R is the reflected light vector R = 2(L.N)N - L
   vec3 R = reflect(-L,N);
   //  V is the view vector (eye vector)
   vec3 V = normalize(View);

   //  Diffuse light is cosine of light and normal vectors
   float Id = max(dot(L,N) , 0.0);
   //  Specular is cosine of reflected and view vectors
   float Is = (Id>0.0) ? pow(max(dot(R,V),0.0) , gl_FrontMaterial.shininess) : 0.0;

   //  Sum color types
   gl_FragColor = gl_FrontMaterial.emission
                + gl_FrontLightProduct[0].ambient
                + Id*gl_FrontLightProduct[0].diffuse
                + Is*gl_FrontLightProduct[0].specular;
}
