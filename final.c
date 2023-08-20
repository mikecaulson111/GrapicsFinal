#include "CSCIx229.h"
#include <time.h>

#define ATan(y,x) (atan2(y,x)*180/3.14159265)




// l -> turn on line mode

double dim = 5;

// For angles overall
int th = 0; //20
int ph = 0; //60
int th1 = 0;
int ph1 = 0;

int ph2 = 0;
int th2 = 0;

int inside = 1; // This is the boolean of being inside the house


//for textures
int mode1 = 0;

double asp = 1;
int fov = 55;

// Many lighting necessities with help from Professor:
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  10;  // Ambient intensity (%)
int diffuse   =  50;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float Ylight = 3;
int sunset = 0; // activates sunset mode

int light = 1;

int w = 500; // width
int h = 500; // height

// Controls viewing mode:
int mode = 1; // 0=perspective, 1=Fist Person

double Ex = 0;   // Eye position in the x axis
double Ez = 9;   // Eye position in the z axis
double dt = 0.1; // Time step

// To control speed:
double tvals [10] = {0.001, 0.01, 0.05, 0.1, 0.5, 1, 1.5, 2.0, 2.5, 5.0};
int tnum = 3; // start at dt = 0.1

int axes = 0;

float eyeposy = 0;
float accy = -0.1;
float vely = 0;

const float Emission[]  = {0.0,0.0,0.0,1.0};
const float Ambient[]   = {0.5,0.5,0.5,1.0};
// const float Ambient[] = {0.03,0.03,0.03,1.0}; // USE THIS IN THE END
const float Diffuse[]   = {1.0,1.0,1.0,1.0};
const float Specular[]  = {1.0,1.0,1.0,1.0};
const float Direction[] = {0.0,-1.0,0.0,1.0};
const float Shinyness[] = {16};



// View single item at a time
int single = 4;

// for fun
int size = 1;

int party = 1;

unsigned int texture[21];

double Cy; // just for now


int shader[] = {0,0,0,0,0};
// changes the shader mode:
int shademode = 2;

// Controls if lights are swinging
int swing = 0;

int lines = 0;

#ifdef GL_VERSION_4_0
#define MODE 11
int shader4[] = {0,0,0}; //  OpenGL 4 shaders
#else
#define MODE 10
#endif

//  Transformation matrixes
float ProjectionMatrix[16];
float ViewMatrix[16];
//  Set lighting parameters using uniforms
float Position[4];

int onoff = 1; // Controls if light is on
int counteronoff = 0; // counts frames between turning on and off

float l1x = 0.0;
float l1y = 20.0;
float l1z = 0.0;


int onner = 1;


static void SetColor(float x, float y, float z) {
    float color[] = {x,y,z,1.0};
    float dark[] = {x/20.,y/20.,z/20.,1.0};
    // float dark[] = {x*0.1,y*0.1,z*0.1,1.0};
    glColor3f(x,y,z);
    // glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,dark);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,color);

}

float Abs(float x) {
    if(x < 0) {
        return -x;
    } else {
        return x;
    }
}


// Using sphere2 from example 8 from class
static void sphere(double x, double y, double z, double r,int spec) {

    // float yellow[] = {1,1,0,1};
    float Emission[] = {0,0,0,0};
    // float Emission[] = {0,0,1,0};
    const int d = 15;

    glMaterialf(GL_FRONT,GL_SHININESS,shiny);
    // glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
    if(spec == 0) {
        glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
    }

    glPushMatrix();

    glTranslated(x,y,z);
    glScaled(r,r,r);

    glColor3f(1,1,1);
    // SetColor(1,1,1);
    for(int ph = -90; ph < 90; ph += d) {
        glBegin(GL_QUAD_STRIP);
        for(int th = 0; th <= 360; th += d) {
            float p1[] = {Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph)};
            float p2[] = {Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d)};

            // With normals:
            glNormal3f(p1[0],p1[1],p1[2]);
            glTexCoord2d(p1[0],p1[1]); glVertex3f(p1[0],p1[1],p1[2]);

            glNormal3f(p2[0],p2[1],p2[2]);
            glTexCoord2d(p2[0],p2[1]); glVertex3f(p2[0],p2[1],p2[2]);
        }
        glEnd();
    }

    glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
    glPopMatrix();
    // glDisable(GL_TEXTURE_2D);
}

void Rectang(double xlow, double ylow, double zlow, double xhigh, double yhigh, double zhigh,int s, int t) {
    double dx = xhigh-xlow;
    double dy = yhigh-ylow;
    double dz = zhigh-zlow;
    
    glPushMatrix();
    glTranslated(xlow,ylow,zlow);
    glScaled(dx,dy,dz);

    glBegin(GL_QUADS);

    // front side
    glNormal3f(0,0,1);
    glTexCoord2f(0,t); glVertex3d(0, 1,  1);
    glTexCoord2f(0,0); glVertex3d(0, 0, 1);
    glTexCoord2f(s,0); glVertex3d(1, 0, 1);
    glTexCoord2f(s,t); glVertex3d(1, 1,  1);


    // bottom
    glNormal3f(0,-1,0);
    glTexCoord2f(0,t); glVertex3d(0,  0, 1);
    glTexCoord2f(0,0); glVertex3d(0,  0, 0);
    glTexCoord2f(s,0); glVertex3d(1, 0, 0);
    glTexCoord2f(s,t); glVertex3d(1, 0, 1);

    // top
    glNormal3f(0,1,0);
    glTexCoord2f(0,t); glVertex3d(0,  1, 1);
    glTexCoord2f(0,0); glVertex3d(0,  1, 0);
    glTexCoord2f(s,0); glVertex3d(1, 1, 0);
    glTexCoord2f(s,t); glVertex3d(1, 1, 1);

    // left side:
    glNormal3f(-1,0,0);
    glTexCoord2f(0,t); glVertex3d(0, 1,  0);
    glTexCoord2f(0,0); glVertex3d(0, 0, 0);
    glTexCoord2f(s,0); glVertex3d(0, 0, 1);
    glTexCoord2f(s,t); glVertex3d(0, 1,  1);

    // right side:
    glNormal3f(1,0,0);
    glTexCoord2f(0,t); glVertex3d(1, 1,  1);
    glTexCoord2f(0,0); glVertex3d(1, 0, 1);
    glTexCoord2f(s,0); glVertex3d(1, 0, 0);
    glTexCoord2f(s,t); glVertex3d(1, 1,  0);

    // back side:
    glNormal3f(0,0,-1);
    glTexCoord2f(0,t); glVertex3d(1, 1,  0);
    glTexCoord2f(0,0); glVertex3d(1, 0, 0);
    glTexCoord2f(s,0); glVertex3d(0,   0, 0);
    glTexCoord2f(s,t); glVertex3d(0,   1,  0);

    glEnd();

    glPopMatrix();
}

void Cylinder(double x, double y, double z, double r, double h,double th, double ph) {
    glPushMatrix();
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glRotated(ph,1,0,0);
    // glScaled(r,h,r);
    glBegin(GL_QUAD_STRIP);
    for(int th = 0; th <= 360; th += 30) {
        glNormal3f(Cos(th), 0, Sin(th));
        glTexCoord2f(3*(360./th),0); glVertex3d(r*Cos(th), 0, r*Sin(th));
        glTexCoord2f(3*(360./th),0.98);glVertex3d(r*Cos(th), h, r*Sin(th));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,1,0);
    glVertex3d(0,h,0);
    for(int th = 0; th <= 360; th += 30) {
        glVertex3d(r*Cos(th),h,r*Sin(th));
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,-1,0);
    glVertex3d(0,0,0);
    for(int th = 0; th <= 360; th += 30) {
        glVertex3d(r*Cos(th),0,r*Sin(th));
    }
    glEnd();
    glPopMatrix();

}

//Board:
// l is the length of the nail
void Board(double x, double y, double z, double dx, double dy, double dz, int th, int ph, double l) {
    glPushMatrix();

    glTranslated(x,y,z);
    glRotated(ph,0,1,0);
    glRotated(th,0,0,1);
    glScaled(dx,dy,dz);
    glBindTexture(GL_TEXTURE_2D,texture[4]);
    SetColor(181./255.,101./255.,29./255.);
    Rectang(-1,-0.25,-0.0625,1,0.25,0.0625,1,1);
    // put studs here
    glBindTexture(GL_TEXTURE_2D,texture[8]);
    SetColor(170./255.,169./255.,173./255.);
    Cylinder(-0.9375,-0.125,0.0625,0.04,l,0,270);
    Cylinder(-0.9375,-0.125,0.0625,0.05,0.0625,0,90);

    Cylinder(0.9375,-0.125,0.0625,0.04,l,0,270);
    Cylinder(0.9375,-0.125,0.0625,0.05,0.0625,0,90);

    Cylinder(-0.9375,0.125,0.0625,0.04,l,0,270);
    Cylinder(-0.9375,0.125,0.0625,0.05,0.0625,0,90);

    Cylinder(0.9375,0.125,0.0625,0.04,l,0,270);
    Cylinder(0.9375,0.125,0.0625,0.05,0.0625,0,90);

    glPopMatrix();
}

// Create a generic house
void house(double x, double y, double z, double dx, double dy, double dz, double ph) {
    
    // float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    float almostwhite[] = {0.8,0.8,0.8};


    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,black);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
    
    
    glPushMatrix();

    glTranslated(x,y+dy,z);
    glRotated(ph,0,1,0);
    glScaled(dx,dy,dz);

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode1?GL_REPLACE:GL_MODULATE);
    // Binds the side of the house to a brick texture
    glBindTexture(GL_TEXTURE_2D,texture[0]);


    // Color for side of house
    // SetColor(252.0/255.0, 107.0/255.0, 3.0/255.0);
    SetColor(80./255.,25./255.,33./255.);

    glBegin(GL_QUADS);

    // Side squares:
    // Right side:
    glNormal3f(inside==1?-1:1,0,0);
    glTexCoord2f(0,0); glVertex3d(+1,-1,+1);
    glTexCoord2f(2,0); glVertex3d(+1,-1,-1);
    glTexCoord2f(2,2); glVertex3d(+1,+1,-1);
    glTexCoord2f(0,2); glVertex3d(+1,+1,+1);

    // Left side:
    glNormal3f(inside==1?1:-1,0,0);
    glTexCoord2f(0,0); glVertex3d(-1,-1,-1);
    glTexCoord2f(2,0); glVertex3d(-1,-1,+1);
    glTexCoord2f(2,2); glVertex3d(-1,+1,+1);
    glTexCoord2f(0,2); glVertex3d(-1,+1,-1);

    glEnd();

    // glDisable(GL_TEXTURE_2D);
    // Bottom in brown
    // glColor3f(1, 248/255, 220/255);
    SetColor(1,1,1);
    glBindTexture(GL_TEXTURE_2D,texture[5]);


    glBegin(GL_QUADS);
    glNormal3f(0,1,0);
    glTexCoord2f(10,10); glVertex3d(+1,-1,+1);
    glTexCoord2f(0,10); glVertex3d(-1,-1,+1);
    glTexCoord2f(0,0); glVertex3d(-1,-1,-1);
    glTexCoord2f(10,0); glVertex3d(+1,-1,-1);
    glEnd();

    // glEnable(GL_TEXTURE_2D);
    // Binds texture to roof shingles texture
    glBindTexture(GL_TEXTURE_2D,texture[1]);

    SetColor(150.0/255.0, 75.0/255.0, 0);

    glBegin(GL_QUADS);
    // ROOF:

    // right roof top:
    glNormal3f(1.5625, 0.3125,0);
    glTexCoord2f(0,0); glVertex3d(+1.25,+0.975,+1.25);
    glTexCoord2f(1,0); glVertex3d(+1.25,+0.975,-1.25);
    glTexCoord2f(1,1); glVertex3d(0,+1.6,-1.25);
    glTexCoord2f(0,1); glVertex3d(0,+1.6,+1.25);

    //right roof bottom:
    glNormal3f(-1.5625, -0.3125,0); 
    glTexCoord2f(0,0); glVertex3d(+1.25,+0.875,+1.25);
    glTexCoord2f(1,0); glVertex3d(+1.25,+0.875,-1.25);
    glTexCoord2f(1,1); glVertex3d(0,+1.5,-1.25);
    glTexCoord2f(0,1); glVertex3d(0,+1.5,+1.25);

    // left roof top:
    glNormal3f(-1.5625,0.3125,0);
    glTexCoord2f(0,0); glVertex3d(-1.25,+0.975,-1.25);
    glTexCoord2f(1,0); glVertex3d(-1.25,+0.975,+1.25);
    glTexCoord2f(1,1); glVertex3d(0,+1.6,+1.25);
    glTexCoord2f(0,1); glVertex3d(0,+1.6,-1.25);

    // left roof bottom:
    glNormal3f(+1.5625,-0.3125,0);
    glTexCoord2f(0,0); glVertex3d(-1.25,+0.875,-1.25);
    glTexCoord2f(1,0); glVertex3d(-1.25,+0.875,+1.25);
    glTexCoord2f(1,1); glVertex3d(0,+1.5,+1.25);
    glTexCoord2f(0,1); glVertex3d(0,+1.5,-1.25);
    glEnd();

    // Back to the bricks for the chimney
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    // Chimney Color
    SetColor(178./255., 34./255., 34./255.);

    glBegin(GL_QUADS);


    // Chimney:
    // Left side
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0); glVertex3d(0.625, 1, 0.625);
    glTexCoord2f(0.25,0); glVertex3d(0.625, 1, 0.875);
    glTexCoord2f(0.25,0.5); glVertex3d(0.625, 1.75, 0.875);
    glTexCoord2f(0,0.5); glVertex3d(0.625, 1.75, 0.625);

    // Right side:
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3d(0.875,1,0.875);
    glTexCoord2f(0.25,0); glVertex3d(0.875,1,0.625);
    glTexCoord2f(0.25,0.5); glVertex3d(0.875,1.75,0.625);
    glTexCoord2f(0,0.5); glVertex3d(0.875,1.75,0.875);

    // Front side:
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3d(0.625,1,0.875);
    glTexCoord2f(0.25,0); glVertex3d(0.875,1,0.875);
    glTexCoord2f(0.25,0.5); glVertex3d(0.875,1.75,0.875);
    glTexCoord2f(0,0.5); glVertex3d(0.625,1.75,0.875);

    // Back side:
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3d(0.875,1,0.625);
    glTexCoord2f(0.25,0); glVertex3d(0.625,1,0.625);
    glTexCoord2f(0.25,0.5); glVertex3d(0.625,1.75,0.625);
    glTexCoord2f(0,0.5); glVertex3d(0.875,1.75,0.625);

    glEnd();

    // Puts a picture frame on top of the chimney which makes it look open on top
    // glBindTexture(GL_TEXTURE_2D,texture[9]);
    glBegin(GL_QUADS);
    // Top
    glNormal3f(0,1,0);
    glTexCoord2f(0,0); glVertex3d(0.625, 1.75, 0.875);
    glTexCoord2f(1,0); glVertex3d(0.875, 1.75, 0.875);
    glTexCoord2f(1,1); glVertex3d(0.875, 1.75, 0.625);
    glTexCoord2f(0,1); glVertex3d(0.625, 1.75, 0.625);
    glEnd();

    // More roofing texture
    glBindTexture(GL_TEXTURE_2D,texture[1]);

    SetColor(150.0/255.0, 75.0/255.0, 0);

    glBegin(GL_QUADS);

    // Front of roof:

    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(-1.25,0.875,1.25);
    glTexCoord2f(1,0); glVertex3f(0,1.5,1.25);
    glTexCoord2f(1,1); glVertex3f(0,1.6,1.25);
    glTexCoord2f(0,1); glVertex3f(-1.25,0.975,1.25);

    glTexCoord2f(0,0); glVertex3f(0,1.5,1.25);
    glTexCoord2f(1,0); glVertex3f(1.25,0.875,1.25);
    glTexCoord2f(1,1); glVertex3f(1.25,0.975,1.25);
    glTexCoord2f(0,1); glVertex3f(0,1.6,1.25);

    //Back of roof:
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(0,1.5,-1.25);
    glTexCoord2f(1,0); glVertex3f(-1.25,0.875,-1.25);
    glTexCoord2f(1,1); glVertex3f(-1.25,0.975,-1.25);
    glTexCoord2f(0,1); glVertex3f(0,1.6,-1.25);

    glTexCoord2f(0,0); glVertex3f(1.25,0.875,-1.25);
    glTexCoord2f(1,0); glVertex3f(0,1.5,-1.25);
    glTexCoord2f(1,1); glVertex3f(0,1.6,-1.25);
    glTexCoord2f(0,1); glVertex3f(1.25,0.975,-1.25);
    
    //left section on roof:
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0); glVertex3d(-1.25,0.875,-1.25);
    glTexCoord2f(1,0); glVertex3d(-1.25,0.875,1.25);
    glTexCoord2f(1,1); glVertex3d(-1.25,0.975,1.25);
    glTexCoord2f(0,1); glVertex3d(-1.25,0.975,-1.25);

    //right section on roof:
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3d(1.25,0.875,1.25);
    glTexCoord2f(1,0); glVertex3d(1.25,0.875,-1.25);
    glTexCoord2f(1,1); glVertex3d(1.25,0.975,-1.25);
    glTexCoord2f(0,1); glVertex3d(1.25,0.975,1.25);

    glEnd();

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);



    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,black);
    //For front and back of house:
    //front
    // SetColor(252./255.,107./255.,3./255.);
    SetColor(80./255.,25./255.,33./255.);
    // Binds to Brick again for last part of house
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_POLYGON);
    glNormal3f(0,0,inside==1?-1:1);
    glTexCoord2f(0,0); glVertex3d(-1,-1,+1);
    glTexCoord2f(2,0); glVertex3d(+1,-1,+1);
    glTexCoord2f(2,2); glVertex3d(+1,+1,+1);
    // glTexCoord2f(0.5,1.25); glVertex3d(0,+1.5,+1); // steeple
    glTexCoord2f(1,2.5); glVertex3d(0,+1.5,+1); // steeple
    glTexCoord2f(0,2); glVertex3d(-1,+1,+1);
    glEnd();

    // Back
    glBegin(GL_POLYGON);
    glNormal3f(0,0,inside==1?1:-1);
    glTexCoord2f(0,0); glVertex3d(+1,-1,-1);
    glTexCoord2f(2,0); glVertex3d(-1,-1,-1);
    glTexCoord2f(2,2); glVertex3d(-1,+1,-1);
    glTexCoord2f(1,2.5); glVertex3d(0,+1.5,-1); // steeple
    glTexCoord2f(0,2); glVertex3d(+1,+1,-1);
    glEnd();

    // Sets up window textures

    // glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    // glBindTexture(GL_TEXTURE_2D,texture[8]);
    // glEnable(GL_BLEND);
    // // glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // CANNOT GET THIS TO WORK // MAYBE GL_ONE I DONT KNOW
    // glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    // glDepthMask(0);

    // glColor4f(2./255.,200./255.,200./255.,0.5);

    // glBegin(GL_QUADS);

    // //Window on front of house
    // glNormal3f(0,0,inside==1?-1:1);
    // glTexCoord2f(0,0); glVertex3f(-0.75,-0.85,inside==1?0.999:1.001);
    // glTexCoord2f(1,0); glVertex3f(-0.25,-0.85,inside==1?0.999:1.001);
    // glTexCoord2f(1,1); glVertex3f(-0.25,-0.7,inside==1?0.999:1.001);
    // glTexCoord2f(0,1); glVertex3f(-0.75,-0.7,inside==1?0.999:1.001);

    // glEnd();
    // glDisable(GL_BLEND);
    // glDepthMask(1);


    // SetColor(2./255.,200./255.,200./255.);
    SetColor(181./255.,101./255.,29./255.);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,black);
    glBindTexture(GL_TEXTURE_2D,texture[9]);
    glBegin(GL_QUADS);


    //1left
    glNormal3f(inside==1?1:-1,0,0);
    glTexCoord2f(0,0); glVertex3f(inside==1?-0.999:-1.001,-0.85,-0.55);
    glTexCoord2f(0,1); glVertex3f(inside==1?-0.999:-1.001,-0.7,-0.55);
    glTexCoord2f(1,1); glVertex3f(inside==1?-0.999:-1.001,-0.7,-0.25);
    glTexCoord2f(1,0); glVertex3f(inside==1?-0.999:-1.001,-0.85,-0.25);

    //2left
    glNormal3f(inside==1?1:-1,0,0);
    glTexCoord2f(1,0); glVertex3f(inside==1?-0.999:-1.001,-0.85,0.55);
    glTexCoord2f(1,1); glVertex3f(inside==1?-0.999:-1.001,-0.7,0.55);
    glTexCoord2f(0,1); glVertex3f(inside==1?-0.999:-1.001,-0.7,0.25);
    glTexCoord2f(0,0); glVertex3f(inside==1?-0.999:-1.001,-0.85,0.25);

    // //1right
    glNormal3f(inside==1?-1:1,0,0);
    glTexCoord2f(0,0); glVertex3f(inside==1?0.999:1.001,-0.85,-0.55);
    glTexCoord2f(0,1); glVertex3f(inside==1?0.999:1.001,-0.7,-0.55);
    glTexCoord2f(1,1); glVertex3f(inside==1?0.999:1.001,-0.7,-0.25);
    glTexCoord2f(1,0); glVertex3f(inside==1?0.999:1.001,-0.85,-0.25);

    // //2right
    glNormal3f(inside==1?-1:1,0,0);
    glTexCoord2f(1,0); glVertex3f(inside==1?0.999:1.001,-0.85,0.55);
    glTexCoord2f(1,1); glVertex3f(inside==1?0.999:1.001,-0.7,0.55);
    glTexCoord2f(0,1); glVertex3f(inside==1?0.999:1.001,-0.7,0.25);
    glTexCoord2f(0,0); glVertex3f(inside==1?0.999:1.001,-0.85,0.25);
    glEnd();

    // Now to board up the windows:
    glBindTexture(GL_TEXTURE_2D,texture[4]);
    Board(-0.99,-0.77,0.4,.15,.1,.1,50,90,0.150);
    Board(-0.98,-0.77,0.4,.15,.1,.1,310,90,0.210);

    Board(0.99,-0.77,-0.4,.15,.1,.1,50,270,0.150);
    Board(0.98,-0.77,-0.4,.15,.1,.1,315,270,0.210);

    Board(-0.99,-0.77,-0.4,.15,.1,.1,50,90,0.150);
    Board(-0.98,-0.77,-0.4,.15,.1,.1,325,90,0.210);
    Board(-0.97,-0.77,-0.4,0.15,0.1,0.1,90,90,0.310);

    Board(0.99,-0.74,0.4,.17,.1,.1,0,270,0.150);
    Board(0.99,-0.80,0.4,.17,.1,.1,0,270,0.210);
    Board(0.98,-0.77,0.4,.15,.1,.1,50,90,0.210);

    // Bings to Extravagant door texture
    glBindTexture(GL_TEXTURE_2D,texture[2]);

    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,almostwhite); // to make metal door look more shiny(directional)

    SetColor(120./1020.,124./1020.,133./1020.);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glBegin(GL_QUADS);

    // door on front of house
    // float almostwhite[] = {0.8,0.8,0.8};
    glNormal3f(0,0,inside==1?-1:1);
    glTexCoord2f(-1,0); glVertex3d(0.125, -1, inside==1?0.999:1.001);
    glTexCoord2f(1,0); glVertex3d(0.625, -1, inside==1?0.999:1.001);
    glTexCoord2f(1,1); glVertex3d(0.625, -0.6, inside==1?0.999:1.001);
    glTexCoord2f(-1,1); glVertex3d(0.125, -0.6, inside==1?0.999:1.001);


    glEnd();

    glBindTexture(GL_TEXTURE_2D,texture[11]);
    SetColor(0.1,0.1,0.1);
    glNormal3f(0,-1,0);
    glBegin(GL_QUADS);
    glTexCoord2d(0,0); glVertex3f(-1,1,-1);
    glTexCoord2d(10,0); glVertex3f(1,1,-1);
    glTexCoord2d(10,10); glVertex3f(1,1,1);
    glTexCoord2d(0,10); glVertex3f(-1,1,1);
    glEnd();

    // resets matrix
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    ErrCheck("House");
}


// x,y,z is the bottom left point for the bed (on the legboard)
void Bed(double x, double y, double z, double dx, double dz) {

    

    float black[] = {0,0,0,1};


    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,black);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);


    glPushMatrix();

    glTranslated(x,y,z);

    // Baseboard part
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    SetColor(102./255., 51./255., 0);
    Rectang(0,0.25,-2,2.5,0.5,0,4,1);

    // BackBoard
    Rectang(0, 0.45, -2, 2.5, 1.25, -1.875,4,1);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,0,1);
    glVertex3d(1.25, 1.2, -1.875); glTexCoord2f(0.5,0);
    for(int th = 0; th <= 180; th += 15) {
        glVertex3d(1.25+Cos(th), 1.25+Sin(th),-1.875);
        glTexCoord2f(Sin(th/2),Cos(th));
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,0,-1);
    glVertex3d(1.25, 1.2, -2);
    for(int th = 0; th <= 180; th += 15) {
        glVertex3d(1.25+Cos(th), 1.25+Sin(th),-2);
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    for(int th = 0; th <= 180; th += 15) {
        glNormal3f(Cos(th), Sin(th),0);
        glVertex3d(1.25+Cos(th), 1.25+Sin(th), -1.875);
        glVertex3d(1.25+Cos(th), 1.25+Sin(th), -2);
    }
    glEnd();

    // Legs:
    Cylinder(0.125, 0, -0.125, 0.05, 0.25,0,0);
    Cylinder(0.125, 0, -1.875, 0.05, 0.25,0,0);
    Cylinder(2.375, 0, -0.125, 0.05, 0.25,0,0);
    Cylinder(2.375, 0, -1.875, 0.05, 0.25,0,0);

    glBindTexture(GL_TEXTURE_2D, texture[3]);

    // red bed part
    SetColor(136./255.,8./255.,8./255.);
    Rectang(0.125,0.5,-1.875,2.375,0.625,-0.125,4,1);

    // Pillows
    SetColor(1,1,1);
    Rectang(0.375, 0.625, -1.8125, 1.125, 0.75, -1.5625,4,1);
    Rectang(1.375, 0.625, -1.8125, 2.125, 0.75, -1.5625,4,1);

    glBindTexture(GL_TEXTURE_2D,texture[10]);
    glEnable(GL_BLEND);
    glDepthMask(0);
    glNormal3f(0,1,0);
    glBegin(GL_QUADS);
    glTexCoord2d(0,0); glVertex3f(1,0.751,-1.8125);
    glTexCoord2d(1,0); glVertex3f(1,0.751,-1.5625);
    glTexCoord2d(1,1); glVertex3f(1.125,0.751,-1.5625);
    glTexCoord2d(0,1); glVertex3f(1.125,0.751,-1.8125);
    glEnd();
    glDepthMask(1);
    glDisable(GL_BLEND);


    ErrCheck("Bed");
    glPopMatrix();
}

// x,y,z is bottom left side of chair
// th is y rotation and ph is x rotation
void Chair(double x, double y, double z, double dx, double dz, double th, double ph) {
    float black[] = {0,0,0,1};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,black);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();

    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glRotated(ph,1,0,0);
    glScaled(dx,1,dz);

    SetColor(1,1,1);
    glBindTexture(GL_TEXTURE_2D, texture[3]);

    // Bottom cushion:
    Rectang(-0.0375,0.675,-0.525,0.7125+0.0375,0.1125+0.675,0,2*dx,1);
    // Rectang(x,y,z,x,y,z,s,t);


    // Back Cushion
    Rectang(0,0.675,-0.625,0.7125,0.675+0.75,-0.525,2*dx,1);


    // TEXTURE WOOD
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    // Armrests:
    SetColor(78./255.,53./255.,36./255.);
    Rectang(-0.075,0.675,-0.625,0,0.675+0.3375,-0.225,4,1);
    Rectang(0.7125,0.675,-0.625,0.7125+0.075,0.675+0.3375,-0.225,4,1);

    // Backboard
    Rectang(-0.075,0.675,-0.685,0.7125+0.075,0.675+0.85,-0.625,4,1);

    // Base:
    Rectang(-0.075,0.665,-0.685,0.7125+0.075,0.675,0,4,1);

    // Legs:
    Cylinder(0,0,-0.685+0.0375,0.04,0.680,0,0);
    Cylinder(0,0,-0.0375,0.04,0.680,0,0);
    Cylinder(0.7125,0,-0.685+0.0375,0.04,0.680,0,0);
    Cylinder(0.7125,0,-0.0375,0.04,0.680,0,0);

    ErrCheck("Chair");
    glPopMatrix();

}

// This is cented on the MIDDLE NOT THE LEFT BOTTOM
void Table(double x, double y, double z, double dx, double dy, double dz, double th) {
    glPushMatrix();

    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);

    SetColor(1,1,1);
    Rectang(-1,1.2,-4,1,1.3,3,4,1);
    Cylinder(0,0,-0.5,0.1,1.25,0,0);
    Cylinder(0,0,-2.75,0.1,1.25,0,0);
    Cylinder(0,0,1.75,0.1,1.25,0,0);

    ErrCheck("Table");
    glPopMatrix();
}

// x,y,z is bottom left
void Fireplace(double x, double y, double z, double th) {
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glTranslated(x,y,z);
    glRotated(th,0,1,0);

    // Two pillars on left and right:
    Rectang(-1.5,0.25,-1,-1,2.25,0.5,4,1);
    Rectang(1,0.25,-1,1.5,2.25,0.5,4,1);

    // backboard:
    Rectang(-1.5,0.25,-1,1.5,2.25,-0.9,4,1);

    // base step up:
    Rectang(-2,0,-1,2,0.25,2.,4,1);

    // Top massive Brick:
    Rectang(-1.75,1.75,-1,1.75,2.5,0.75,4,1);

    // Chimney:
    Rectang(-0.75,2.4,-1,0.75,20,0,4,20);
    SetColor(150./255.,75./255.,0);
    glBindTexture(GL_TEXTURE_2D,texture[7]);
    // Logs
    for(int i = 0; i < 4; i++) {
        Cylinder(0.5*Cos(90*i),0.25,0.5*Sin(90*i), 0.15,0.75,-90-(90*i),35);
    }
    // Circular blob in middle on fire which glows
    glColor3f(214./255.,0.,0.);
    // float color[] = {214./255.,0,0,0};
    float color1[] = {1.0,0.0,0,1.0};
    glBindTexture(GL_TEXTURE_2D,texture[8]);
    // glMaterialfv(GL_FRONT,GL_EMISSION,color1);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,color1);
    sphere(0,0.35,0,0.3,1);

    ErrCheck("FirePlace");
    glPopMatrix();
}

// Centered at the middle of the bowl
void Bowl(double x, double y, double z, double dx, double dy, double dz, double th) {
    glPushMatrix();
    glTranslated(x,y,z);
    glScaled(dx,dy,dz);
    glRotated(th,1,0,0); // Rotates about the x-axis so can flip over bowles
    SetColor(1,1,1);
    int d = 30;
    // Outside half circle
    for(int ph = -90; ph < 0; ph += d) {
        glBegin(GL_QUAD_STRIP);
        for(int th = 0; th <= 360; th += d) {
            float p1[] = {Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph)};
            float p2[] = {Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d)};

            // With normals:
            glNormal3f(p1[0],p1[1],p1[2]);
            glTexCoord2d(Cos(th),p1[1]); glVertex3f(p1[0],p1[1],p1[2]);

            glNormal3f(p2[0],p2[1],p2[2]);
            glTexCoord2d(Cos(th),p2[1]); glVertex3f(p2[0],p2[1],p2[2]);
        }
        glEnd();
    }
    //Inside half circle
    for(int ph = -90; ph < 0; ph += d) {
        glBegin(GL_QUAD_STRIP);
        for(int th = 0; th <= 360; th += d) {
            float p1[] = {0.9*Sin(th)*Cos(ph) , 0.9*Sin(ph) , 0.9*Cos(th)*Cos(ph)};
            float p2[] = {0.9*Sin(th)*Cos(ph+d) , 0.9*Sin(ph+d) , 0.9*Cos(th)*Cos(ph+d)};

            // With normals:
            glNormal3f(-p1[0],-p1[1],-p1[2]);
            glTexCoord2d(0.9*Cos(th),p1[1]); glVertex3f(p1[0],p1[1],p1[2]);

            glNormal3f(-p2[0],-p2[1],-p2[2]);
            glTexCoord2d(0.9*Cos(th),p2[1]); glVertex3f(p2[0],p2[1],p2[2]);
        }
        glEnd();
    }
    // Up part of bowl
    glBegin(GL_QUAD_STRIP);
    for(int ph = 0; ph <= 360; ph += d) {
        float p1[] = {Cos(ph),Sin(ph)};
        // glNormal3f(Cos(ph),0,Sin(ph));
        glNormal3f(p1[0],0,p1[1]);
        glTexCoord2d(p1[0],0.2); glVertex3f(p1[0],0,p1[1]);

        // glVertex3f(Cos(ph),0.25,Sin(ph));
        glTexCoord2d(p1[0],0); glVertex3f(p1[0],0.25,p1[1]);
    }
    glEnd();
    glBegin(GL_QUAD_STRIP);
    for(int ph = 0; ph <= 360; ph += d) {
        float p1[] = {Cos(ph),Sin(ph)};
        glNormal3f(-Cos(ph),0,-Sin(ph));
        glTexCoord2d(p1[0],0.2); glVertex3f(0.9*Cos(ph),0,0.9*Sin(ph));

        glTexCoord2d(p1[0],0); glVertex3f(0.9*Cos(ph),0.25,0.9*Sin(ph));
    }
    glEnd();
    glBegin(GL_QUAD_STRIP);
    for(int ph = 0; ph <= 360; ph += d) {
        float p1[] = {Cos(ph),Sin(ph)};
        glNormal3f(0,1,0);
        glTexCoord2d(0.9,p1[1]); glVertex3f(0.9*Cos(ph),0.25,0.9*Sin(ph));

        glTexCoord2d(1,p1[1]); glVertex3f(Cos(ph),0.25,Sin(ph));
    }
    glEnd();

    ErrCheck("Bowl");
    glPopMatrix();
}

void Plate(double x, double y, double z, double dx, double dy, double dz, double th) {
    glPushMatrix();
    
    glTranslated(x,y,z);
    glScaled(dx,dy,dz);
    glRotated(th,1,0,0);
    // SetColor(0,1,1);
    SetColor(173./255.,216./255.,230./255.);
    int d = 10;
    for(int ph = -90; ph < 0; ph += d) {
        glBegin(GL_QUAD_STRIP);
        for(int th = 0; th <= 360; th += d) {
            float p1[] = {Sin(th)*Cos(ph) , 0.1*Sin(ph) , Cos(th)*Cos(ph)};
            float p2[] = {Sin(th)*Cos(ph+d) , 0.1*Sin(ph+d) , Cos(th)*Cos(ph+d)};

            // With normals:
            glNormal3f(p1[0],-p1[1],p1[2]);
            glTexCoord2d(Cos(th),p1[1]); glVertex3f(p1[0],p1[1],p1[2]);

            glNormal3f(p2[0],-p2[1],p2[2]);
            glTexCoord2d(Cos(th),p2[1]); glVertex3f(p2[0],p2[1],p2[2]);
        }
        glEnd();
    }

    ErrCheck("Plate");
    glPopMatrix();
}

void Fork(double x, double y, double z, double dx, double dy, double dz, double th, double ph) {
    glBindTexture(GL_TEXTURE_2D,texture[8]);
    
    glPushMatrix();

    glTranslated(x,y,z);
    glRotated(ph,0,1,0);
    glRotated(th,1,0,0);
    glScaled(dx,dy,dz);

    // SetColor(1,1,1);
    SetColor(0.2,0.2,0.2);

    for(int i = 0; i < 5; i++) {
        glPushMatrix();
        float c1 = 0.5;
        float c2 = 0.5; // scaling factor in x and z respectively
        switch(i) {
            case 0:
                glTranslated(-1,1,0);
                glRotated(180,1,0,0);
                c1 = 0.25;
                c2 = 0.25;
                break;
            case 1:
                glTranslated(0,1,0);
                glRotated(180,1,0,0);
                c1 = 0.25;
                c2 = 0.25;
                break;
            case 2:
                glTranslated(1,1,0);
                glRotated(180,1,0,0);
                c1 = 0.25;
                c2 = 0.25;
                break;
            case 3:
                c1 = 1.25;
                c2 = 0.5;
                break;
            default:
            glTranslated(0,-4,0);
                c1 = 0.35;
                c2 = 0.35;
        }


        int d = 30;
        for(int ph1 = -90; ph1 < 0; ph1 += d) {
            glBegin(GL_QUAD_STRIP);
            for(int th1 = 0; th1 <= 360; th1 += d) {
                float p1[] = {c1*Sin(th1)*Cos(ph1) , Sin(ph1) , c2*Cos(th1)*Cos(ph1)};
                float p2[] = {c1*Sin(th1)*Cos(ph1+d) , Sin(ph1+d) , c2*Cos(th1)*Cos(ph1+d)};

                // With normals:
                glNormal3f(p1[0],p1[1],p1[2]);
                glTexCoord2d(Cos(th1),p1[1]); glVertex3f(p1[0],p1[1],p1[2]);

                glNormal3f(p2[0],p2[1],p2[2]);
                glTexCoord2d(Cos(th1),p2[1]); glVertex3f(p2[0],p2[1],p2[2]);
            }
            glEnd();
        }

        glPopMatrix();
    }

    Cylinder(-1,0,0,0.25,1,0,0);
    Cylinder(0,0,0,0.25,1,0,0);
    Cylinder(1,0,0,0.25,1,0,0);

    Cylinder(0,-4,0,0.35,4,0,0);

    ErrCheck("Fork");
    glPopMatrix();
}

void Spoon(double x, double y, double z, double dx, double dy, double dz, double th, double ph) {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,texture[8]);

    glTranslated(x,y,z);
    glRotated(ph,0,1,0);
    glRotated(th,1,0,0);
    glScaled(dx,dy,dz);

    Bowl(0,0,0,0.5,1,0.5,90);
    Cylinder(0,-4,0,0.125,3.1,0,0);
    ErrCheck("Spoon");
    glPopMatrix();
}

void Axe(double x, double y, double z, double dx, double dy, double dz, double th, double ph, double ah,int straps) {
    glPushMatrix();

    glTranslated(x,y,z);
    glRotated(ah,0,0,1);
    glRotated(th,1,0,0);
    glRotated(ph,0,1,0);
    SetColor(192./255.,192./255.,192./255.);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glScaled(dx,dy,dz);

    glBegin(GL_QUADS);

    glNormal3f(-0.009375,0,0.075);
    glTexCoord2d(0,1); glVertex3f(-0.5,2.55,0);
    glTexCoord2d(1,0.6); glVertex3f(0,2.35,0.0625);
    glTexCoord2d(1,0.3); glVertex3f(0,2.2,0.0625);
    glTexCoord2d(0,0); glVertex3f(-0.5,2.05,0);

    glNormal3f(-0.009375,0,-0.075);
    glTexCoord2d(0,1); glVertex3f(-0.5,2.55,0);
    glTexCoord2d(1,0.6); glVertex3f(0,2.35,-0.0625);
    glTexCoord2d(1,0.3); glVertex3f(0,2.2,-0.0625);
    glTexCoord2d(0,0); glVertex3f(-0.5,2.05,0);

    glNormal3f(1,0,0);
    glTexCoord2d(0,0); glVertex3f(0,2.2,0.0625);
    glTexCoord2d(1,0); glVertex3f(0,2.2,-0.0625);
    glTexCoord2d(1,1); glVertex3f(0,2.35,-0.0625);
    glTexCoord2d(0,1); glVertex3f(0,2.35,0.0625);

    glEnd();

    glBegin(GL_TRIANGLES);

    glNormal3f(0.025,0.0625,0);
    glTexCoord2d(0.5,1); glVertex3f(-0.5,2.55,0);
    glTexCoord2d(1,0); glVertex3f(0,2.35,-0.0625);
    glTexCoord2d(0,0); glVertex3f(0,2.35,0.0625);

    glNormal3f(0.025,-0.0625,0);
    glTexCoord2d(0.5,1); glVertex3f(-0.5,2.05,0);
    glTexCoord2d(1,0); glVertex3f(0,2.2,-0.0625);
    glTexCoord2d(0,0); glVertex3f(0,2.2,0.0625);

    glEnd();

    SetColor(78./255.,53./255.,36./255.);
    glBindTexture(GL_TEXTURE_2D,texture[4]);
    Cylinder(-0.0625,0.5,0,0.05,2.0,0,0);

    if(straps == 1) {
        SetColor(0,0,0);
        Cylinder(-0.0625,1.7,0,0.1,0.25,0,0);
    }

    ErrCheck("Axe");

    glPopMatrix();
}

float strappts[] = {1.25,2,0.125,-0.125};

void Sword(double x, double y, double z, double dx, double dy, double dz, double th, double ph, int straps) {
    glPushMatrix();
    // glBindTexture(GL_TEXTURE_2D,texture[8]);
    SetColor(192./255.,192./255.,192./255.);
    glBindTexture(GL_TEXTURE_2D,texture[8]);

    glTranslated(x,y,z);
    glRotated(ph,0,1,0);
    glRotated(th,1,0,0);
    glScaled(dx,dy,dz);

    glBegin(GL_QUADS);

    //Left front blade:
    glNormal3f(-0.125,0,0.25);
    glTexCoord2d(0,0); glVertex3f(-0.125,0.875,0);
    glTexCoord2d(1,0); glVertex3f(0,0.875,0.0625);
    glTexCoord2d(1,1); glVertex3f(0,2.875,0.0625);
    glTexCoord2d(0,1); glVertex3f(-0.125,2.875,0);

    //Right front blade:
    glNormal3f(0.125,0,0.25);
    glTexCoord2d(0,0); glVertex3f(0,0.875, 0.0625);
    glTexCoord2d(1,0); glVertex3f(0.125, 0.875, 0);
    glTexCoord2d(1,1); glVertex3f(0.125,2.875,0);
    glTexCoord2d(0,1); glVertex3f(0,2.875,0.0625);

    // Left back blade:
    glNormal3f(-0.125,0,-0.25);
    glTexCoord2d(0,0); glVertex3f(-0.125,0.875,0);
    glTexCoord2d(1,0); glVertex3f(-0.125, 2.875,0);
    glTexCoord2d(1,1); glVertex3f(0,2.875,-0.0625);
    glTexCoord2d(0,1); glVertex3f(0,0.875,-0.0625);

    //Right back blade:
    glNormal3f(0.125,0,-0.25);
    glTexCoord2d(0,0); glVertex3f(0,0.875,-0.0625);
    glTexCoord2d(1,0); glVertex3f(0,2.875,-0.0625);
    glTexCoord2d(1,1); glVertex3f(0.125,2.875,0);
    glTexCoord2d(0,1); glVertex3f(0.125,0.875,0);

    glEnd();

    glBegin(GL_TRIANGLES);

    //Front left:
    // 0.015625, -0.0078125, -0.03125
    glNormal3f(-0.015625,0.0078125,0.03125);
    glTexCoord2d(0,0); glVertex3f(-0.125,2.875,0);
    glTexCoord2d(0,0.5); glVertex3f(0,2.875,0.0625);
    glTexCoord2d(1,0.5); glVertex3f(0,3.125,0);

    //Front right
    glNormal3f(0.015625,0.0078125,0.03125);
    glTexCoord2d(0,0); glVertex3f(0.125,2.875,0);
    glTexCoord2d(0,0.5); glVertex3f(0,3.125,0);
    glTexCoord2d(1,0.5); glVertex3f(0,2.875,0.0625);

    //Back left:
    glNormal3f(-0.015625,0.0078125,-0.03125);
    glTexCoord2d(0,0); glVertex3f(-0.125,2.875,0);
    glTexCoord2d(0,0.5); glVertex3f(0,3.125,0);
    glTexCoord2d(1,0.5); glVertex3f(0,2.875,-0.0625);

    //Back right:
    glNormal3f(0.015625,0.0078125,-0.03125);
    glTexCoord2d(0,0); glVertex3f(0.125,2.875,0);
    glTexCoord2d(0,0.5); glVertex3f(0,2.875,-0.0625);
    glTexCoord2d(1,0.5); glVertex3f(0,3.125,0);

    glEnd();

    SetColor(78./255.,53./255.,36./255.);
    glBindTexture(GL_TEXTURE_2D,texture[4]);
    Rectang(-0.25,0.85,-0.125,0.25,0.875,0.125,1,1);
    Cylinder(0,0.5,0,0.08,0.35,0,0);

    if(straps == 1) {
        SetColor(0,0,0);
        glBegin(GL_QUADS);

        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 2; j++) {
                float yer = strappts[i] + strappts[2+j];
                glNormal3f(0,j==0?1:-1,0);
                glVertex3f(0,yer,0.0625);
                glVertex3f(0,yer,0.125);
                glVertex3f(0.1875,yer,0);
                glVertex3f(0.125,yer,0);

                glVertex3f(0,yer,0.0625);
                glVertex3f(0,yer,0.125);
                glVertex3f(-0.1875,yer,0);
                glVertex3f(-0.125,yer,0);

                glVertex3f(0,yer,-0.0625);
                glVertex3f(0,yer,-0.125);
                glVertex3f(0.1875,yer,0);
                glVertex3f(0.125,yer,0);

                glVertex3f(0,yer,-0.0625);
                glVertex3f(0,yer,-0.125);
                glVertex3f(-0.1875,yer,0);
                glVertex3f(-0.125,yer,0);

                if(j == 1) {
                    glNormal3f(0.015625,0,0.0234275);
                    glVertex3f(0,strappts[i]+strappts[2],0.125);
                    glVertex3f(0,strappts[i] + strappts[3], 0.125);
                    glVertex3f(0.1875,strappts[i]+strappts[3],0);
                    glVertex3f(0.1875,strappts[i]+strappts[2],0);

                    glNormal3f(0.015625,0,-0.0234275);
                    glVertex3f(0,strappts[i]+strappts[2],-0.125);
                    glVertex3f(0,strappts[i] + strappts[3], -0.125);
                    glVertex3f(0.1875,strappts[i]+strappts[3],0);
                    glVertex3f(0.1875,strappts[i]+strappts[2],0);

                    glNormal3f(-0.015625,0,0.0234275);
                    glVertex3f(0,strappts[i]+strappts[2],0.125);
                    glVertex3f(0,strappts[i] + strappts[3], 0.125);
                    glVertex3f(-0.1875,strappts[i]+strappts[3],0);
                    glVertex3f(-0.1875,strappts[i]+strappts[2],0);

                    glNormal3f(-0.015625,0,-0.0234275);
                    glVertex3f(0,strappts[i]+strappts[2],-0.125);
                    glVertex3f(0,strappts[i] + strappts[3], -0.125);
                    glVertex3f(-0.1875,strappts[i]+strappts[3],0);
                    glVertex3f(-0.1875,strappts[i]+strappts[2],0);
                }

            }
        }

        glEnd();
    }

    ErrCheck("Sword");
    glPopMatrix();
}

void Cabinet(double x, double y, double z, double dx, double dy, double dz,double ph) {
    glPushMatrix();

    glTranslated(x,y,z);
    glRotated(ph,0,1,0);
    glScaled(dx,dy,dz);

    glBindTexture(GL_TEXTURE_2D,texture[4]);
    // Skirt
    glBegin(GL_QUADS);
    glNormal3f(0,0.28125,-0.5625);
    glTexCoord2d(0.125,0.5); glVertex3f(-1.125,0.25,-0.5);
    glTexCoord2d(0,0); glVertex3f(-1.25,0,-0.625);
    glTexCoord2d(1,0); glVertex3f(1.25,0,-0.625);
    glTexCoord2d(0.875,0.5); glVertex3f(1.125,0.25,-0.5);

    glNormal3f(0,0.28125,0.5625);
    glTexCoord2d(0.125,0.5); glVertex3f(-1.125,0.25,0.5);
    glTexCoord2d(0,0); glVertex3f(-1.25,0,0.625);
    glTexCoord2d(1,0); glVertex3f(1.25,0,0.625);
    glTexCoord2d(0.875,0.5); glVertex3f(1.125,0.25,0.5);

    glNormal3f(-0.25,0.125,0);
    glTexCoord2d(0.125,0.5); glVertex3f(-1.125,0.25,-0.5);
    glTexCoord2d(0,0); glVertex3f(-1.25,0,-0.625);
    glTexCoord2d(1,0); glVertex3f(-1.25,0,0.625);
    glTexCoord2d(0.875,0.5); glVertex3f(-1.125,0.25,0.5);

    glNormal3f(0.25,0.125,0);
    glTexCoord2d(0.125,0.5); glVertex3f(1.125,0.25,-0.5);
    glTexCoord2d(0,0); glVertex3f(1.25,0,-0.625);
    glTexCoord2d(1,0); glVertex3f(1.25,0,0.625);
    glTexCoord2d(0.875,0.5); glVertex3f(1.125,0.25,0.5);
    glEnd();

    //Base
    Rectang(-1.125,0.25,-0.5,1.125,1.25,0.5,2,1);
    // Actual Cabinet
    Rectang(-1.125,1.25,-0.5,1.125,3,-0.45,1,1);
    Rectang(-1.125,1.25,-0.5,-1.075,3,0.5,1,1);
    Rectang(1.075,1.25,-0.5,1.125,3,0.5,1,1);
    Rectang(-1.125,2.95,-0.5,1.125,3,0.5,1,1);

    // Drawers
    Rectang(-1.075,0.3,0.55,1.075,0.7,0.6,1,1);
    Rectang(-1.075,0.8,0.55,1.075,1.2,0.6,1,1);
    Rectang(-1.025,0.33,0.56,1.025,0.65,0.4,1,1);
    Rectang(-1.025,0.83,0.56,1.025,1.15,0.4,1,1);

    // Handles:
    sphere(0,1,0.65,0.1,0);
    sphere(0.5375,0.5,0.65,0.1,0);
    sphere(-0.5375,0.5,0.65,0.1,0);

    ErrCheck("cabinet");
    glPopMatrix();
}

void Painting(double x, double y, double z, double dx, double dy, double dz, double dimx, double dimy, int th, int ph, int ah) {

    glPushMatrix();

    SetColor(1,1,1);

    glTranslated(x,y,z);
    glRotated(ph,0,1,0);
    glRotated(ah,0,0,1);
    glRotated(th,1,0,0);
    glScaled(dx,dy,dz);

    // Need to call the bind texture before calling this function!
    Rectang(-dimx/2,-dimy/2,0,dimx/2,dimy/2,0.125,1,1);
    glBindTexture(GL_TEXTURE_2D,texture[8]);
    SetColor(212./255.,175./255.,55./255.);
    glBegin(GL_QUADS);

    glNormal3f(-0.125,0,0.0625);
    glVertex3f(-dimx/2 - 0.0625, -dimy/2 - 0.0625, 0);
    glVertex3f(-dimx/2, -dimy/2, 0.125);
    glVertex3f(-dimx/2, dimy/2, 0.125);
    glVertex3f(-dimx/2 - 0.0625, dimy/2 + 0.0625, 0);

    glNormal3f(0.125,0,0.0625);
    glVertex3f(dimx/2, -dimy/2, 0.125);
    glVertex3f(dimx/2 + 0.0625, -dimy/2 - 0.0625, 0);
    glVertex3f(dimx/2 + 0.0625, dimy/2 + 0.0625, 0);
    glVertex3f(dimx/2, dimy/2, 0.125);

    glNormal3f(0,0.125,0.0625);
    glVertex3f(-dimx/2, dimy/2, 0.125);
    glVertex3f(dimx/2, dimy/2, 0.125);
    glVertex3f(dimx/2 + 0.0625, dimy/2 + 0.0625, 0);
    glVertex3f(-dimx/2 - 0.0625, dimy/2 + 0.0625, 0);

    glNormal3f(0,-0.125,0.0625);
    glVertex3f(-dimx/2 - 0.0625, -dimy/2 - 0.0625, 0);
    glVertex3f(dimx/2 + 0.0625, -dimy/2 - 0.0625, 0);
    glVertex3f(dimx/2, -dimy/2, 0.125);
    glVertex3f(-dimx/2, -dimy/2, 0.125);

    //Back:
    glNormal3f(0,0,-1);
    glVertex3f(-dimx/2 - 0.0625,-dimy/2 - 0.0625,0);
    glVertex3f(dimx/2 + 0.0625, -dimy/2 -0.0625, 0);
    glVertex3f(dimx/2 + 0.0625, dimy/2 + 0.0625, 0);
    glVertex3f(-dimx/2 - 0.0625, dimy/2 + 0.0625, 0);

    glEnd();

    ErrCheck("Painting");
    glPopMatrix();
}

void ExitSign(double xlow, double ylow, double zlow, double xhigh, double yhigh, double zhigh,int s, int t) {
    float red[] = {1.0,0.0,0.0,0.0};
    float black[] = {0.0,0.0,0.0,0.0};
    if(swing == 1) {
        glMaterialfv(GL_FRONT,GL_EMISSION,red);
    }
    Rectang(xlow,ylow,zlow,xhigh,yhigh,zhigh,s,t);
    glMaterialfv(GL_FRONT,GL_EMISSION,black);

    ErrCheck("ExitSign");
}

float platesz[] = {0.1,1.6,3.1,4.6};

float swordposy[] = {25,17,13,15.5,29,20,43,23,10,18,23.75};
float remember[] = {15,17,13,15.5,15.75,20,43,23,10,18,23.75};
float swordposx[] = {3,  2.5, 3,    -3,  -2.5, -6, 7,    8,   4,    -6.6, -8.7};
float swordposz[] = {-7, 6,   -4.5, 3.7, -8.6, -5.5,  -4.7, 8.9, -5.6, 7.6,  2.3};
float swordvely[] = {0,0,0,0,0,0,0,0,0,0,0};

float picturey = 8;
float picturevely = 0;


/*
 * Draw scene using the OpenGL2 style
 */
void Scene2()
{
    if(lines) {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
    // Trying to get the light to flicker
    // counteronoff += 1;
    // if(counteronoff >= 20) {
    //     // printf("Here\n");
    //     srand(time(0));
    //     int temprandom = rand() % 100;
    //     if(temprandom <= 83) {
    //         onoff = 1;
    //     } else {
    //         onoff = 0;
    //     }
    //     counteronoff = 0;
    // }
   //  Undo previous transformations
   glLoadIdentity();

   //  orthogonal - set world position
//    if(mode == 0) {
//         glRotatef(ph,1,0,0);
//         glRotatef(th,0,1,0);
//    } else 
    if (mode==0)// perspective, set eye position
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  First person - set first person orientation
   else
   {
        double Cx = +2*Sin(th+180)*Cos(ph+180) + Ex;
        Cy = -2*Sin(ph+180) + eyeposy;
        double Cz = -2*Cos(th+180)*Cos(ph+180) + Ez;
        gluLookAt(Ex,eyeposy,Ez, Cx, Cy, Cz, 0,1,0);
   }

   //  Draw light position as sphere (still no lighting here)
   float Position[]  = {4*Cos(zh),Ylight,4*Sin(zh),1.0};
//    float Position1[] = {4*Cos(zh+180),4*Sin(zh+180),0,1.0};
    // float Position1[] = {Ex,eyeposy,Ez,1.0};
    float Position1[4];
    float Position2[4];
    float Position3[4];
    float Direction1[] = {0.4*Cos(zh)*Sin(zh),-1.0,0.4*Sin(zh),1.0};
    float Direction2[] = {0.4*Cos(zh),-1.0,0.4*Sin(zh),1.0};
    float Direction3[] = {0.4*Sin(zh),-1.0,0.4*Cos(zh),1.0};
    if(swing == 0) {
        Direction1[0] = 0.0;
        Direction1[1] = -1.0;
        Direction1[2] = 0.0;

        Direction2[0] = 0.0;
        Direction2[1] = -1.0;
        Direction2[2] = 0.0;

        Direction3[0] = 0.0;
        Direction3[1] = -1.0;
        Direction3[2] = 0.0;

    }
    if(onoff) {
        Position1[0] = l1x;
        Position1[1] = l1y;
        Position1[2] = l1z;
        Position1[3] = 1.0;

        Position2[0] = l1x;
        Position2[1] = l1y;
        Position2[2] = l1z-5;
        Position2[3] = 1.0;

        Position3[0] = l1x;
        Position3[1] = l1y;
        Position3[2] = l1z+5;
        Position3[3] = 1.0;
    } else {
        Position1[0] = 0;
        Position1[1] = -1000.0;
        Position1[2] = 0;
        Position1[3] = 1.0;

        Position2[0] = 0;
        Position2[1] = -1000.0;
        Position2[2] = 0;
        Position2[3] = 1.0;

        Position3[0] = 0;
        Position3[1] = -1000.0;
        Position3[2] = 0;
        Position3[3] = 1.0;
    }
   glColor3f(1,1,1);
   if(shademode==2) {
        glPushMatrix();
        glTranslated(Position1[0],Position1[1],Position1[2]);
        sphere(0,0,0,0.1,0);
        glPopMatrix();

        glPushMatrix();
        glTranslated(Position2[0],Position2[1],Position2[2]);
        sphere(0,0,0,0.1,0);
        glPopMatrix();

        glPushMatrix();
        glTranslated(Position3[0],Position3[1],Position3[2]);
        sphere(0,0,0,0.1,0);
        glPopMatrix();

        // glPushMatrix();
        // glTranslated(Position1[0],-Position1[1],Position1[2]);
        // sphere(0,0,0,0.1);
        // glPopMatrix();
   } else if(shademode==0 || shademode == 1){
    glPushMatrix();
    glTranslated(Position[0],Position[1],Position[2]);
    sphere(0,0,0,0.3,0);
    glPopMatrix();
   }

   // Second light for fun
   //  OpenGL should normalize normal vectors
   glEnable(GL_NORMALIZE);
   //  Enable lighting
   glEnable(GL_LIGHTING);
    if(shademode==2) {
        float cutoff[1];
        if(swing == 1) {
            cutoff[0] = 3.14 / 15;
        } else {
            cutoff[0] = 3.14 / 18;
        }
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
        glEnable(GL_LIGHT3);
        glLightfv(GL_LIGHT1,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT1,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT1,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT1,GL_POSITION,Position1);
        glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,Direction1);
        glLightfv(GL_LIGHT1,GL_SPOT_CUTOFF,cutoff);// need to make it a GL_FLOAT??

        glLightfv(GL_LIGHT2,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT2,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT2,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT2,GL_POSITION,Position2);
        glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,Direction2);
        glLightfv(GL_LIGHT2,GL_SPOT_CUTOFF,cutoff);

        glLightfv(GL_LIGHT3,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT3,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT3,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT3,GL_POSITION,Position3);
        glLightfv(GL_LIGHT3,GL_SPOT_DIRECTION,Direction3);
        glLightfv(GL_LIGHT3,GL_SPOT_CUTOFF,cutoff);
    } else {
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
        glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,Direction);
    }
    ErrCheck("Lighting");

   //  Set materials
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

   //
   //  Draw scene
   //
   //  Select shader (0 => no shader)
   glUseProgram(shader[shademode]);

    // Draws house
    house(0, 0, 0, 10, 10, 10, 0);

    // Draws bed in corner (it has a spider on it but the texture looks weird)
    SetColor(1,1,1);
    Bed(-9.5,0,-8,1,0.5);

    // Switches chairs from being in correct place to being all messed up (in crazy moode)
    if(swing == 0) {
        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 4; j++) {
                if(i == 0) {
                    Chair(-0.75,0,platesz[j]+0.425,1,1,90,0);
                } else {
                    Chair(0.75,0,platesz[j]-0.425,1,1,270,0);
                }
            }
        }
    } else {
        Chair(-0.75,0,platesz[0]+0.425,1,1,90,0);
        Chair(-1.4,0,platesz[1]+0.425,1,1,90,0);
        Chair(-0.75,0.75,platesz[2]+0.425,1,1,75,270);
        Chair(-2.5,0.5,platesz[3]+0.425,1,1,90,125);
        Chair(1.85,0,platesz[0]-0.425,1,1,180,0);
        Chair(0.75,0,platesz[1]-0.425,1,1,270,0);
        Chair(1.35,0,platesz[2]-0.425,1,1,270+45,0);
        Chair(0.75,0,platesz[3]-0.425,1,1,270,0);
    }

    // Draws table in the middle of the room
    Table(0, 0, 3, 1, 1, 1, 0); 

    // Draws a fire place in the back of the room with some glowing embers(the blob in the middle)
    Fireplace(0,0,-9,0);

    // Draws plates, bowls, forks, and spoons, where forks and bowls change with crazy mode
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 4; j++) {
            glBindTexture(GL_TEXTURE_2D,texture[4]);
            // Bowl(i==0?-0.65:0.65,swing==1?1.35:1.45,platesz[j],0.125,0.125,0.125,swing==1?180:0);
            Bowl(i==0?swing==1&&j%2==1?-0.65+0.45:-0.65:swing==1&&j%2==0?0.65-0.45:0.65,swing==1?1.35:1.45,platesz[j],0.125,0.125,0.125,swing==1?180:0);
            Plate(i==0?-0.65:0.65,1.35,platesz[j],0.25,0.125,0.25,0);
            glBindTexture(GL_TEXTURE_2D,texture[8]);
            if(i==0) {
                Fork(-0.65,1.35,platesz[j] - 0.35,0.0125,0.04,0.04,swing==1?180:90,90);
                Spoon(-0.63,1.35,platesz[j] + 0.35,0.05,0.05,0.05,270,270);
            } else {
                Fork(0.65,1.35,platesz[j] + 0.35,0.0125,0.04,0.04,swing==1?160:90,270);
                Spoon(0.63,1.35,platesz[j] - 0.35,0.05,0.05,0.05,270,90);
            }
        }
    }

    // Draws some axes and swords around the room 
    SetColor(192./255.,192./255.,192./255.);
    Axe(0,1.8,1,0.5,0.5,0.5,110,90,30,0);
    glBindTexture(GL_TEXTURE_2D,texture[8]);
    Sword(0,1.45,2,0.5,0.5,0.5,95,0,0);

    glBindTexture(GL_TEXTURE_2D,texture[8]);
    Sword(0.25,2.25,-8.72,0.5,0.5,0.5,-10,0,0);
    glBindTexture(GL_TEXTURE_2D,texture[8]);
    Axe(-0.25,2.25,-8.74,0.5,0.5,0.5,-10,0,swing==1?65:0,0);

    // draws swords that fall from the air when crazy mode is active
    if(swing==1) {
        for(int i = 0; i < 11; i++) {
            glBindTexture(GL_TEXTURE_2D,texture[8]);
            Sword(swordposx[i], swordposy[i],swordposz[i],0.5,0.5,0.5,180,0,0);
        }
    }

    // Place a couch near the bed side of the house
    if(swing == 1) {
        Chair(-8.4,0.75,0.85,3,1,90,270);
    } else {
        Chair(-8.75,0,0.85,3,1,90,0);
    }
    // Places a coffee table that is a scaled down version of Larger table
    Table(-7.5,0,0,0.5,0.5,0.5,0);

    // Build a cabinet and place the sword, axe, and a painting inside (sword and axe have straps holding it up)
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    Cabinet(9.375,0,0,1,1,1,270);
    Sword(9.73,2.9,0.5,0.4,0.45,0.45,180,90,1);
    Sword(9.5,1.26,-0.25,0.25,0.2,0.25,90,-35,0);
    Axe(9.73,1.2,-0.5,0.7,0.6,0.7,0,90,0,1);

    // Places some paintings around the room which have two modes
        // one for normal mode, then when crazy mode they change and some fall
    glBindTexture(GL_TEXTURE_2D,texture[12]);
    Painting(9.7,1.36,-0.80,1,1,1,.256,.192,-45,270,0);

    glBindTexture(GL_TEXTURE_2D,swing==1?texture[16]:texture[13]);
    // Painting(5,swing==1?3.75:8,-9.875,20,20,1,0.168,0.256,0,0,0);
    Painting(5,picturey,-9.875,20,20,1,0.168,0.256,0,0,0);

    glBindTexture(GL_TEXTURE_2D,swing==1?texture[15]:texture[14]);
    Painting(-5,8,-9.875,20,20,1,0.176,0.256,0,0,swing==1?25:0);

    glBindTexture(GL_TEXTURE_2D,swing==1?texture[17]:texture[18]);
    Painting(-5,4,9.875,10,10,1,.206,.256,0,180,0);

    glBindTexture(GL_TEXTURE_2D,texture[20]);
    Painting(-7.5,0.6,0,1,1,0.5,0.300,0.300,270,225,0);

    // Places an exit sign over door which is only lit up when crazy mode is active
    glBindTexture(GL_TEXTURE_2D,texture[19]);
    ExitSign(3.25,4,9.75,4.25,5.0,9.9999,1,1);

    ErrCheck("Scene");

   //  Revert to fixed pipeline
   glUseProgram(0);

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
//    glColor3f(1,1,1);
    SetColor(1,1,1);
   if (axes)
   {
      const double len=2.0;  //  Length of axes
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
}


// Controls the rotations of the scene
void special(int key, int x, int y) {
    if(key == GLUT_KEY_RIGHT) {
        th += 5;
        th2+=5;
    } else if (key == GLUT_KEY_LEFT) {
        th -= 5;
        th2-=5;
    } else if (key == GLUT_KEY_UP) {
        if(ph < 90) {
            ph += 5;
        }
    } else if (key == GLUT_KEY_DOWN) {
        if(ph > -85) {
            ph -= 5;
        }
    } 

    ph %= 360;
    th %= 360;

    // if(mode == 0) {
    //     Project(0,asp,dim);
    // } else {
    //     Project(fov,asp,dim);
    // }
    Project(fov,asp,dim);

    glutPostRedisplay();
}


void keys(unsigned char ch, int x, int y) {
    if(ch == 27) {
        exit(0);
    } else if (ch == 'm') {
        // if(mode == 0) { // Orthogonal to Perspective
        //     mode = 1;
        // } else 
        if (mode == 0){ // Perspective to First Person
            ph1 = ph; // Saves the polar angle to restore when switching back
            th1 = th;
            mode = 1;
            // dim=10;
            // Initially set the position of the eye in line with current screen
            Ex = -2*Sin(th1)*Cos(ph1) * dim;
            Ez = +2*Cos(th1)*Cos(ph1) * dim;
            ph = 0;
        } else { // First Person to Perspective
            // dim = 9;
            ph = ph1;
            th = ATan(-Ex,Ez);
            mode = 0;
        }
    } else if (ch == 'w') { // Walk forward

        // Find looking direction
        double dx = 2*Sin(th);
        double dz = -2*Cos(th);

        // Move in that direction
        Ex = Ex + (dt * dx);
        Ez = Ez + (dt * dz);
    } else if (ch == 's') { // Walk backward
        // Find looking direction
        double dx = +2*Sin(th);
        double dz = -2*Cos(th);

        // Move opposite that direction
        Ex = Ex - (dt * dx);
        Ez = Ez - (dt * dz);
    } else if (ch == 'a') {
        // Getting looking direction but looking left to crabwalk
        double dx = +2*Sin(th - 90);
        double dz = -2*Cos(th - 90);

        Ex = Ex + dt*dx;
        Ez = Ez + dt*dz;


    } else if (ch == 'd') {
        // Getting looking direction but looking right to crabwalk
        double dx = +2*Sin(th + 90);
        double dz = -2*Cos(th + 90);

        Ex = Ex + dt*dx;
        Ez = Ez + dt*dz;
    } else if (ch == 'k') {
        shademode += 1;
        shademode %= 4;
        if(shademode == 0) {
            shademode = 1;
        }
    } else if (ch == ' ') {
        vely = 1;
    } else if (ch == 'Y') {
        Ylight += 0.5;
    } else if (ch == 'y') {
        Ylight -= 0.5;
    } else if (ch == '?') {
        if(eyeposy == 2) {
            eyeposy = 1;
        } else {
            eyeposy = 2;
        }
    } else if (ch == ']') {
        l1x += 0.1;
    } else if (ch == '[') {
        l1x -= 0.1;
    } else if (ch == '+') {
        l1y += 0.1;
    } else if (ch == '-') {
        l1y -= 0.1;
    } else if (ch == ';') {
        onoff = 0;
    } else if (ch == '<') {
        if(onner == 1 ) {
            onner = 0;
        } else {
            onner = 1;
        }
    } else if(ch =='l') {
        if(lines ==1) {
            lines = 0;
        } else {
            lines = 1;
        }
    } else if(ch == 'A') {
        axes += 1;
        axes %= 2;
    } else if(ch == 'c') {
        swing += 1;
        swing %= 2;
        if(swing == 1) {
            for(int i = 0; i < 11; i++) {
                swordposy[i] = remember[i];
                swordvely[i] = 0;
            }
        } else {
            picturevely = 0;
            picturey = 8;
        }
    } 

    if(mode == 0) {
        Project(0,asp,dim);
    } else {
        Project(fov,asp,dim);
    }
    glutPostRedisplay();
}

// Reshape function
void reshape(int width,int height)
{
   //  Set the viewport to the entire window
    asp = (height>0) ? (double)width/height : 1;
    glViewport(0,0, RES*width,RES*height);
    // Calls the Project to deal with different projections:
    if(mode == 0) {
        Project(0,asp,dim);
    } else {
        Project(fov,asp,dim);
    }

    w = width;
    h = height;
}

// Display the scene
void display() {
    // glClearColor(0.3,0.5,1.0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    Scene2();

    if(axes) {
        if(mode == 0) {
            glWindowPos2i(5,5);
            Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s, k=%d",th,ph,dim,fov,"Perpective",shademode);
        } else {
            glWindowPos2i(5,5);
            Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s, k=%d",th,ph,dim,fov,"First Person",shademode);
            glWindowPos2i(5,20);
            Print("Cy=%f",Cy);
        }
    }

    ErrCheck("Display");
    glFlush();
    glutSwapBuffers();
}

void idle()
{
    if(Abs(Ex) <= 10 && Abs(Ez) <= 10) {
        inside = 1;
    } else {
        inside = 0;
    }

    if(mode == 2) { // Controls jumping in first person
        if(eyeposy <= 2 && (vely == 0 || vely < 0)) {
            eyeposy = 2;
            vely = 0;
        } else {
            vely += accy;
            eyeposy += vely;
        }
    }

    if(swing == 1) { // Controls gravity on swords and painting
        for(int i = 0; i < 11; i++) {
            if(swordposy[i] > 0.75) {
                swordvely[i] += accy;
                swordvely[i] *= 0.95;
                swordposy[i] += swordvely[i];
            } else {
                swordposy[i] = 0.75;
            }
        }
        if(picturey > 3.75) {
            picturevely += accy;
            picturey += picturevely;
        } else {
            picturey = 3.75;
        }
    }

   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
   if(eyeposy > 2) {
    vely += accy;
    eyeposy += vely;
   } else if (eyeposy <= 2) {
    if(vely > 0) {
        vely += accy;
        eyeposy += vely;
    } else {
        eyeposy = 2;
    }
   }
}

/*
 *  Read text file
 */
char* ReadText(char *file)
{
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   int n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}

/*
 *  Print Shader Log
 */
void PrintShaderLog(int obj,char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) Fatal("Error compiling %s\n",file);
}

/*
 *  Print Program Log
 */
void PrintProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}

/*
 *  Create Shader
 */
int CreateShader(GLenum type,char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   fprintf(stderr,"Compile %s\n",file);
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Return name
   return shader;
}

/*
 *  Create Shader Program
 */
int CreateShaderProg(char* VertFile,char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   int vert = CreateShader(GL_VERTEX_SHADER,VertFile);
   //  Create and compile fragment shader
   int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
   //  Attach vertex shader
   glAttachShader(prog,vert);
   //  Attach fragment shader
   glAttachShader(prog,frag);
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}




int main(int argc, char* argv[]) {

    // Initialize the whole scene with my name in the window
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(500,500);

    glutCreateWindow("Final Project - Michael Caulson");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif


    srand(time(0));
    glutSpecialFunc(special);

    glutKeyboardFunc(keys);

    glutDisplayFunc(display);

    glutReshapeFunc(reshape);

    glutIdleFunc(idle);

    //  Switch font to nearest
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   //  Create Shader Programs
//    shader[1] = CreateShaderProg("toon.vert","toon.frag");
//    shader[2] = CreateShaderProg("pixlight.vert","pixlight.frag");
//    shader[3] = CreateShaderProg("pixtex.vert","pixtex.frag");
// //    shader[4] = CreateShaderProg("pixlightspot.vert","pixlightspot.frag");
//     shader[4] = CreateShaderProg("pixlightspotup.vert","pixlightspotup.frag");

    shader[1] = CreateShaderProg("pixtex.vert","pixtex.frag");
    shader[2] = CreateShaderProg("pixlightspotup.vert","pixlightspotup.frag");
    shader[3] = CreateShaderProg("pixlightspot.vert","pixlightspot.frag");


    // Loads Textures
    // texture[0] = LoadTexBMP("brick.bmp");
    texture[0] = LoadTexBMP("brick.bmp");
    texture[1] = LoadTexBMP("roof1.bmp");
    texture[2] = LoadTexBMP("door.bmp");
    texture[3] = LoadTexBMP("newBlanket.bmp");
    texture[4] = LoadTexBMP("newOak.bmp");
    texture[5] = LoadTexBMP("newercobble.bmp");
    texture[6] = LoadTexBMP("stone.bmp");
    texture[7] = LoadTexBMP("newbark.bmp");
    texture[8] = LoadTexBMP("Steel.bmp");
    texture[9] = LoadTexBMP("shutter.bmp");
    texture[10] = LoadTexBMP("webb.bmp");
    texture[11] = LoadTexBMP("ceiling.bmp");
    texture[12] = LoadTexBMP("doodle.bmp");
    texture[13] = LoadTexBMP("spookyhouse.bmp");
    texture[14] = LoadTexBMP("spookyboy.bmp");
    texture[15] = LoadTexBMP("ghost1.bmp");
    texture[16] = LoadTexBMP("ghost2.bmp");
    texture[17] = LoadTexBMP("ghost3.bmp");
    texture[18] = LoadTexBMP("woman.bmp");
    texture[19] = LoadTexBMP("exitsign.bmp");
    texture[20] = LoadTexBMP("cub.bmp");

    ErrCheck("Main");

    glutMainLoop();

    return 0;
}