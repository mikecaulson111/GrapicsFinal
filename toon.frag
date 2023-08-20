//  Cartoon fragment shader
//  Adapted from Lighthouse3D
#version 120

varying vec3 N;
varying vec3 L;
// varying vec3 L2;
	
void main()
{
   float intensity = dot(normalize(N),normalize(L));
   // float intensity2 = dot(normalize(N),normalize(L2));

   float f;
   if (intensity > 0.95)
      f= 1.0;
   else if (intensity > 0.5)
      f = 0.6;
   else if (intensity > 0.25)
      f = 0.4;
   else
      f = 0.2;

   // Once again for second light, testing it out
   // float f2;
   // if (intensity2 > 0.95)
   //    f2= 1.0;
   // else if (intensity2 > 0.5)
   //    f2 = 0.6;
   // else if (intensity2 > 0.25)
   //    f2 = 0.4;
   // else
   //    f2 = 0.2;


   // gl_FragColor = f*vec4(1.0,0.5,0.5,1.0) + f2*vec4(1.0,0.5,0.5,1.0);
   gl_FragColor = f*vec4(1.0,0.5,0.5,1.0);
}
