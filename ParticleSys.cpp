#include "Generator.h"
#include "Spring.h"
#include <vector>

#define WINDOW_WIDTH    1080
#define WINDOW_HEIGHT   1080
#define STARTX 0.5
#define STARTY 1.0
#define STARTZ 0.5
#define RADIUS 0.03
#define FLOORHEIGHT 0
#define BOXLEFT 0
#define BOXRIGHT 1
#define BOXHEIGHT 1
#define BOXFRONT 1
#define BOXBACK 0
#define EPS 0.08

static double WinWidth = WINDOW_WIDTH;
static double WinHeight = WINDOW_HEIGHT;
static Vector3d origin(STARTX, STARTY, STARTZ);
static Vector3d V0(0.1,0.1,0.1);

const double timestep = 0.05;
const static int noOfParticles = 8;
const static int noOfSprings = ((noOfParticles-1)*noOfParticles)/2;
const double mass = 0.01;

const double k = 0.09;
const double d = 0.01;
const double l = 0.8;
static double cameraX=0.5, cameraY=0.0, cameraZ=5.5;
static GLfloat sphere_diffuseX = 0.411, sphere_diffuseY = 0.411, sphere_diffuseZ = 0.411, sphere_diffuseA = 1.0;
Vector3d Acceleration(0.0,0.0,0.0);
Vector3d Force(0.0,0.0,0.0);
static std::vector<Particle> particles;
static std::vector<Particle> oldparticles;
static std::vector<Spring> springs;

static Generator generator(origin, noOfParticles, timestep);

void do_material()
{
    
    float mat_ambient[] = {0.0,0.0,0.0,1.0};
    float mat_diffuse[] = {0.8, 1.0, 1.0,1.0};
    float mat_specular[] = {0.03,0.03,0.03,1.0};
    float mat_shininess[] = {1.0};
    
    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
    
}

void do_lights()
{
    
    float light0_ambient[] = {0.0, 0.0, 0.0, 0.0};
    float light0_diffuse[] = {2.0, 2.0, 2.0, 0.0};
    float light0_specular[] = {2.25, 2.25, 2.25, 0.0};
    float light0_position[] = {1.5, 2.0, 2.0, 1.0};
    float light0_direction[] = { -1.5, -2.0, -2.0, -2.0, 1.0};
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light0_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    
    glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
}

void setup_the_viewvolume()
{
    
    Vector3d eye, view, up;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(37.0, 1.0, 0.1, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    eye.x = cameraX;
    eye.y = cameraY;
    eye.z = cameraZ;
    view.x = 0.5;
    view.y = 0.2;
    view.z = 0.0;
    up.x = 0.0;
    up.y = 1.0;
    up.z = 0.0;
    
    gluLookAt(eye.x, eye.y, eye.z, view.x, view.y, view.z, up.x, up.y, up.z);
    
}

void doReshape(int w, int h)
{
    
    int vpw, vph;
    float aspect = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);
    
    if(float(w) / float(h) > aspect)
    {
        vph = h;
        vpw = int(aspect * h + 0.5);
    }
    else
    {
        vpw = w;
        vph = int(w / aspect + 0.5);
    }
    
    glViewport(0, 0, vpw, vph);
    WinWidth = w;
    WinHeight = h;
    setup_the_viewvolume();
    
}

void initParticleSystem()
{
    particles.reserve(noOfParticles);
    oldparticles.reserve(noOfParticles);
    springs.reserve(noOfSprings);
}

double RandomDouble(double a, double b)
{
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = b - a;
    double r = random * diff;
    return a + r;
}

void addParticle(const Vector3d position, const Vector3d velocity)
{
    std::cout<<"adding particle"<<std::endl;
    oldparticles.push_back(Particle (position, velocity, Acceleration, mass, Force));
    particles.push_back(Particle (position, velocity, Acceleration, mass, Force));
    
}

void addParticles()
{
    
    double vx = RandomDouble(0.0,0.01);
    double vy = RandomDouble(0.0,0.01);
    double vz = RandomDouble(0.0,0.01);
    
    Vector3d V2(vx,vy,vz);
    
    Vector3d cube[noOfParticles] = {Vector3d(0.0,0.0,1.0), Vector3d(0.0,1.0,1.0), Vector3d(1.0,1.0,1.0), Vector3d(1.0,0.0,1.0),Vector3d(0.0,0.0,0.0), Vector3d(0.0,1.0,0.0), Vector3d(1.0,1.0,0.0),Vector3d(1.0,0.0,0.0)};
    
    for (int i= 0; i< noOfParticles; i++)
    {
        addParticle(cube[i], V2);
        
    }
}

void addSpring(int i, int j)
{
    int pi = i;
    int pj = j;
    
    springs.push_back(Spring (k, d, l, pi, pj));
   
}


void addSprings()
{
    int count = 0;
    for (int i= 0; i< noOfParticles; i++)
    {
       for (int j=0; j< noOfParticles; j++)
       {
           if (i>j)
           {
               addSpring(i,j);
                count++;
           }
       }
        
    }
}

void particleInteraction(const int i, const int j)
{
    double r = 0.0;
    Vector3d u;
    Vector3d f;
    double f1;
    double f2;
    
    r = (oldparticles[j].Position-oldparticles[i].Position).norm();
    u = (oldparticles[j].Position-oldparticles[i].Position).normalize();
    
    f1 = k* (r-l);
    f2 = d*((oldparticles[j].Velocity- oldparticles[i].Velocity)*u);
    
    f = (f1+f2)*u;
    std::cout<<u.x<<":"<<u.y<<":"<<u.z<<std::endl;
    particles[i].Force = f;
    
    particles[i].Acceleration = oldparticles[i].Force/oldparticles[i].mass;
    
}

void updateParticle(const double timestep, const int i)
{
    
    for (int j=0; j< noOfParticles; j++)
    {   if(i!=j)
        particleInteraction(i,j);
    }
    std::cout<<"i"<<i<<"Velocity"<<oldparticles[i].Acceleration.x<<std::endl;
    particles[i].Velocity = oldparticles[i].Velocity + timestep*particles[i].Acceleration;
    particles[i].Position.x = oldparticles[i].Position.x + (oldparticles[i].Velocity.x * timestep);
    particles[i].Position.y = oldparticles[i].Position.y + (oldparticles[i].Velocity.y * timestep);
    particles[i].Position.z = oldparticles[i].Position.z + (oldparticles[i].Velocity.z * timestep);
    
}


void do_particle_lights()
{
    
    GLfloat sphere_diffuse[] = { sphere_diffuseX, sphere_diffuseY, sphere_diffuseZ, sphere_diffuseA};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_diffuse);
}

void do_spring_setup()
{
    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);
    
    glLineWidth(2.0);
}

void DrawParticle (float x, float y, float z)
{
    /*
     do_particle_lights();
     glBegin(GL_POINTS);
     glVertex3f(x,y,z);
     glEnd();
    
    */
    
    float drawx = x;
    float drawy = y;
    float drawz = z;
    float drawrad = 0.04;
    
    do_particle_lights();
    
    glPushMatrix();
    glTranslatef (drawx,drawy,drawz);
    glutSolidSphere(drawrad,40,40);
    glPopMatrix ();
    
    glFlush();
    
}


void DrawSpring(const Vector3d a, const Vector3d b)
{
    do_spring_setup();
    glBegin(GL_LINES);
    //draw line from vertex a to b
    glColor3f(sphere_diffuseX, sphere_diffuseY, sphere_diffuseZ);
    glVertex3f(a.x, a.y, a.z);
    glColor3f(sphere_diffuseX, sphere_diffuseY, sphere_diffuseZ);
    glVertex3f(b.x, b.y, b.z);
    glEnd();
    glFlush();
    std::cout<<"drawing spring"<<std::endl;
}



void simulateParticle(const double timestep, const int i)
{
    updateParticle(timestep,i);
    DrawParticle(oldparticles[i].Position.x, oldparticles[i].Position.y, oldparticles[i].Position.z);
    
}

void simulateSpring(const double timestep, const int i, const int j)
{
    DrawSpring(oldparticles[i].Position, oldparticles[j].Position);
}



void runGenerator(Generator &generator)
{
    
    for (int i= 0; i< noOfParticles; i++)
    {
        simulateParticle(timestep, i);
        
        for (int j =0; j< noOfParticles; j++)
        {
            
            if(i>j)
            {
                
                simulateSpring(timestep, i,j);
            }
        }
        
    }
    
    oldparticles = particles;
}


void drawDisplay()
{
   
    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0.8,1.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    runGenerator(generator);
    
    glutSwapBuffers();
    
}

void Timer(int unused)
{
    glutPostRedisplay();
    glutTimerFunc(20, Timer, 0);
}

void handleKey(unsigned char key, int x, int y)
{
    double delta = 0.5;
    
    switch (key) {
        case 'a' :
            cameraX -= delta;
            break;
        case 'd' :
            cameraX += delta;
            break;
        case 'w' :
            cameraY += delta;
            break;
        case 's' :
            cameraY -= delta;
            break;
        case 'q' :
            cameraZ += delta;
            break;
        case 'e' :
            cameraZ -= delta;
            break;
        case 'r' :
            sphere_diffuseX = 1.0, sphere_diffuseY = 0.2, sphere_diffuseZ = 0.0, sphere_diffuseA = 1.0;
            break;
        case 'g' :
            sphere_diffuseX = 0.2, sphere_diffuseY = 0.8, sphere_diffuseZ = 0.2, sphere_diffuseA = 1.0;
            break;
        case 'b' :
            sphere_diffuseX = 0.188, sphere_diffuseY = 0.188, sphere_diffuseZ = 1.0, sphere_diffuseA = 1.0;
            break;
        case 'o' :
            sphere_diffuseX = 0.411, sphere_diffuseY = 0.411, sphere_diffuseZ = 0.411, sphere_diffuseA = 1.0;
            break;
        case 27:
            exit(0);
            break;
            
        default:
            return;
            break;
       
    }
    setup_the_viewvolume();
    glutPostRedisplay();
    
}


int main(int argc, char* argv[])
{
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100,50);
    glutCreateWindow("Springy structures");
    glutReshapeFunc(doReshape);
    do_lights();
    do_material();
    
    initParticleSystem();
    addParticles();
    addSprings();
    
    glutDisplayFunc(drawDisplay);
    Timer(0);
    glutKeyboardFunc(handleKey);
    
    glutMainLoop();
    return 0;
    
}

