#include "CSCIx229.h"


void house(double x, double y, double z, double dx, double dy, double dz, double ph, float shiny) {
    
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    float almostwhite[] = {0.8,0.8,0.8};


    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,black);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
    
    
    glPushMatrix();

    glTranslated(x,y,z);
    glRotated(ph,0,1,0);
    glScaled(dx,dy,dz);

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode1?GL_REPLACE:GL_MODULATE);
    // Binds the side of the house to a brick texture
    glBindTexture(GL_TEXTURE_2D,texture[0]);

    glBegin(GL_QUADS);

    //Color of side of house:
    // if(randomcolor == 0) {
    //     glColor3ub(randomColors[whichColor + 0],randomColors[whichColor + 1],randomColors[whichColor + 2]);
    // } else if (randomcolor == 1) {
    //     glColor3f(1,1,0);
    // } else {
    //     glColor3ub(252,107,3);
    // }
    glColor3ub(252,107,3);
    // Side squares:
    // Right side:
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3d(+1,-1,+1);
    glTexCoord2f(1,0); glVertex3d(+1,-1,-1);
    glTexCoord2f(1,1); glVertex3d(+1,+1,-1);
    glTexCoord2f(0,1); glVertex3d(+1,+1,+1);

    // Left side:
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0); glVertex3d(-1,-1,-1);
    glTexCoord2f(1,0); glVertex3d(-1,-1,+1);
    glTexCoord2f(1,1); glVertex3d(-1,+1,+1);
    glTexCoord2f(0,1); glVertex3d(-1,+1,-1);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    // Bottom in brown
    // glColor3f(1, 248/255, 220/255);
    glBegin(GL_QUADS);
    glColor3ub(139,69,19);
    glNormal3f(0,-1,0);
    glVertex3d(+1,-1,+1);
    glVertex3d(-1,-1,+1);
    glVertex3d(-1,-1,-1);
    glVertex3d(+1,-1,-1);

    glEnd();

    glEnable(GL_TEXTURE_2D);
    // Binds texture to roof shingles texture
    glBindTexture(GL_TEXTURE_2D,texture[1]);

    glBegin(GL_QUADS);
    // ROOF:
    glColor3ub(150, 75, 0);
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

    glBegin(GL_QUADS);


    // Chimney:
    glColor3ub(178,34,34);
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
    glBindTexture(GL_TEXTURE_2D,texture[9]);
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

    glBegin(GL_QUADS);

    // Front of roof:
    glColor3ub(139,69,19);

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

    // Sets up window textures
    glBindTexture(GL_TEXTURE_2D,texture[4]);
    glBegin(GL_QUADS);

    // Window on front of house (I want to make this look better, its kinda funny with just a window)
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    // glColor3ub(2,200,220);
    glColor3ub(245,245,245);
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(-0.75,-0.75,1.001);
    glTexCoord2f(1,0); glVertex3f(-0.25,-0.75,1.001);
    glTexCoord2f(1,1); glVertex3f(-0.25,-0.25,1.001);
    glTexCoord2f(0,1); glVertex3f(-0.75,-0.25,1.001);

    //1left
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0); glVertex3f(-1.001,-0.75,-0.75);
    glTexCoord2f(0,1); glVertex3f(-1.001,-0.25,-0.75);
    glTexCoord2f(1,1); glVertex3f(-1.001,-0.25,-0.25);
    glTexCoord2f(1,0); glVertex3f(-1.001,-0.75,-0.25);

    //2left
    glNormal3f(-1,0,0);
    glTexCoord2f(1,0); glVertex3f(-1.001,-0.75,0.75);
    glTexCoord2f(1,1); glVertex3f(-1.001,-0.25,0.75);
    glTexCoord2f(0,1); glVertex3f(-1.001,-0.25,0.25);
    glTexCoord2f(0,0); glVertex3f(-1.001,-0.75,0.25);

    // //1right
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3f(1.001,-0.75,-0.75);
    glTexCoord2f(0,1); glVertex3f(1.001,-0.25,-0.75);
    glTexCoord2f(1,1); glVertex3f(1.001,-0.25,-0.25);
    glTexCoord2f(1,0); glVertex3f(1.001,-0.75,-0.25);

    // //2right
    glNormal3f(1,0,0);
    glTexCoord2f(1,0); glVertex3f(1.001,-0.75,0.75);
    glTexCoord2f(1,1); glVertex3f(1.001,-0.25,0.75);
    glTexCoord2f(0,1); glVertex3f(1.001,-0.25,0.25);
    glTexCoord2f(0,0); glVertex3f(1.001,-0.75,0.25);
    glEnd();
    // glEnable(GL_TEXTURE_2D);

    // Bings to Extravagant door texture
    glBindTexture(GL_TEXTURE_2D,texture[2]);

    glBegin(GL_QUADS);

    // door on front of house
    // float almostwhite[] = {0.8,0.8,0.8};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,almostwhite); // to make metal door look more shiny(directional)
    glColor3ub(120,124,133);
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3d(0.125, -1, 1.001);
    glTexCoord2f(1,0); glVertex3d(0.625, -1, 1.001);
    glTexCoord2f(1,1); glVertex3d(0.625, 0, 1.001);
    glTexCoord2f(0,1); glVertex3d(0.125, 0, 1.001);


    glEnd();



    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,black);
    //For front and back of house:
    //front
    // if(randomcolor == 0) {
    //     glColor3ub(randomColors[whichColor + 0],randomColors[whichColor + 1],randomColors[whichColor + 2]);
    //     whichColor += 1;
    //     whichColor %= 3;
    // } else if (randomcolor == 1){
    //     glColor3f(1,1,0);
    // } else {
    //     glColor3ub(252,107,3);
    // }
    glColor3ub(252,107,3);
    // Binds to Brick again for last part of house
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_POLYGON);
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3d(-1,-1,+1);
    glTexCoord2f(1,0); glVertex3d(+1,-1,+1);
    glTexCoord2f(1,1); glVertex3d(+1,+1,+1);
    glTexCoord2f(0.5,1.25); glVertex3d(0,+1.5,+1); // steeple
    glTexCoord2f(0,1); glVertex3d(-1,+1,+1);
    glEnd();

    // Back
    glBegin(GL_POLYGON);
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3d(+1,-1,-1);
    glTexCoord2f(1,0); glVertex3d(-1,-1,-1);
    glTexCoord2f(1,1); glVertex3d(-1,+1,-1);
    glTexCoord2f(0.5,1.25); glVertex3d(0,+1.5,-1); // steeple
    glTexCoord2f(0,1); glVertex3d(+1,+1,-1);
    glEnd();

    // resets matrix
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    ErrCheck("House");
}