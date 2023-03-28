#include "RayTracing.h"



const int AA_SAMPLES = 1;
const int RAYTRACING_MAXDEPTH = 0;
const int RAYTRACING_MULTIPLE_MAXDEPTH = 0;


/** Metode raytracing principal
Aquesta funcio transforma cada pixel a coordenades de mon, construeix el raig,
envia el raig des de la camera en aquella direccio cridant al metode RayColor
per acabar pintant la imatge final:
Precondicio:
La Scene ha d'estar creada i inicialitzada
**/
void RayTracing::rendering() {
    // Recorregut de cada pixel de la imatge final
    for (int y = cam->viewportY-1; y >= 0; y--) {

        std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;  // Progrés del càlcul

        for (int x = 0; x < cam->viewportX; x++) {

            vec3 col(0, 0, 0);
            //Llancem rajos aleatoris i fem la seva suma ponderada
            for (int s= 0; s<AA_SAMPLES; s++){
                double aa = (linearRand(0.0,10.0)/10-0.5);
                double bb = (linearRand(0.0,10.0)/10-0.5);
                float u = (float(x+aa)) / float(cam->viewportX);
                float v = (float(y+bb)) / float(cam->viewportY);

                Ray r = cam->getRay(u, v);

                col += scene->RayColor(cam->getLookFrom(), r, RAYTRACING_MAXDEPTH, RAYTRACING_MAXDEPTH, RAYTRACING_MULTIPLE_MAXDEPTH); //Sumem tots els colors obtinguts.

            }
            col = col / (float)AA_SAMPLES; //Dividim pel nombre de mostres preses.
            col = sqrt(col); //Gamma correction
            setPixelColor(col, x, y);
        }
    }
    std::cerr << "\nDone.\n";
}

void RayTracing::exitRender() {

}

// Metode de suport que fa el rendering per a tots els Renders que no usen GLUT
void RayTracing::startRendering(int argc, char **argv) {
    renderingScene();
}

void RayTracing::renderingScene()  {
    initRender(filename.toStdString());
    rendering();
    exitRender();
}

void RayTracing::setCamera(shared_ptr<Camera> c) {
    cam = c;
}
void RayTracing::setScene(shared_ptr<Scene> s) {
    scene = s;
}

void RayTracing::setOutputFile(QString output) {
    filename = output;
}



