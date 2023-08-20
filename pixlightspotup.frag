//  Per Pixel Lighting shader from professor modified to be spotlight
#version 120

// const vec3 LDir = vec3(0.0,-1.0,0.0);

varying vec3 View;
varying vec3 Light[3];
varying vec3 Normal;
varying vec3 LDir[3];
varying float cutoff[3];
uniform sampler2D tex;



void main()
{
    // initialize to 0,0,0,0 so base case is no lighting
    vec4 color = vec4(0.0,0.0,0.0,0.0);
    for(int i =0; i < 3; i++) {
        vec3 L = normalize(Light[i]);
        vec3 sd = normalize(LDir[i]);
        vec3 N = normalize(Normal);
        vec3 V = normalize(View);
        if(dot(-L,sd) > cos(cutoff[i])) { // if the dot product is inside the cuttoff, then do lighting
        // if(dot(-V,sd) > 0.9) {
            vec3 R = reflect(-L,N);
            //  Diffuse light is cosine of light and normal vectors
            float Id = max(dot(L,N) , 0.0);
            //  Specular is cosine of reflected and view vectors
            float Is = (Id>0.0) ? pow(max(dot(R,V),0.0) , gl_FrontMaterial.shininess) : 0.0;


            color += gl_FrontMaterial.emission
                    + gl_FrontLightProduct[i].ambient
                    + Id*gl_FrontLightProduct[i].diffuse
                    + Is*gl_FrontLightProduct[i].specular;

            

        }
    }
    // If color still is 0 vector then do ambient lighting, else light scene and use texture
    color = color==vec4(0.0,0.0,0.0,0.0)?gl_FrontMaterial.emission + gl_FrontLightProduct[0].ambient: color;
    gl_FragColor = color * texture2D(tex,gl_TexCoord[0].xy);
}
