//  Per Pixel Lighting shader from professor modified to be Flashlight
#version 120

varying vec3 View;
varying vec3 Light;
varying vec3 Normal;
varying vec3 LDir;
uniform sampler2D tex;



void main()
{
    vec3 L = normalize(Light);
    vec3 sd = normalize(LDir);
    vec3 N = normalize(Normal);
    vec3 V = normalize(View);
    if(dot(-L,sd) > cos(3.14/9)) { // do lighting only if light and spotlight dir are in a cone
    // if(dot(-V,sd) > 0.9) {
        vec3 R = reflect(-L,N);
        //  Diffuse light is cosine of light and normal vectors
        float Id = max(dot(L,N) , 0.0);
        //  Specular is cosine of reflected and view vectors
        float Is = (Id>0.0) ? pow(max(dot(R,V),0.0) , gl_FrontMaterial.shininess) : 0.0;


        vec4 color = gl_FrontMaterial.emission
                + gl_FrontLightProduct[0].ambient
                + Id*gl_FrontLightProduct[0].diffuse
                + Is*gl_FrontLightProduct[0].specular;

        //  Apply texture
        gl_FragColor = color * texture2D(tex,gl_TexCoord[0].xy);


    } else {
        vec4 color = gl_FrontMaterial.emission + gl_FrontLightProduct[0].ambient;
        gl_FragColor = color * texture2D(tex,gl_TexCoord[0].xy);
    }
}
