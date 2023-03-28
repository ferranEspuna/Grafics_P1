#include "Main.h"

enum opcio { Tetera, DuesEsferes, Cilindre,Bcn, Data, Data2Values, DataSph, Box, Polynomial, Europa, Transparencies, LlumCon, Penombres, AmbientOcclusion, Earth, Stars, EarthWithStars, EarthWithStarsAndMoon, TransparentSpheres, DefocusBlur};



int main(int argc, char **argv) {

    cout << "Init..." <<endl;
    Q_INIT_RESOURCE(resources);


    opcio visualitzar = opcio::Earth;




    QString dataPath;
    QString visPath;

    switch(visualitzar){
    case Tetera:{
        dataPath = "://resources/teapot";
        visPath = "://resources/setupRenderTeapot";
        break;
    }
    case DuesEsferes:{
        dataPath = "://resources/twoSpheres";
        visPath = "://resources/setupRenderTwoSpheres";
        break;
    }
    case Cilindre:{ 
        dataPath = "://resources/oneCylinder";
        visPath = "://resources/setupRenderOneCylinder";
        break;
    }
    case Box:{ 
        dataPath = "://resources/oneBox";
        visPath = "://resources/setupRenderOneBox";
        break;
    }
    case Data:{
        dataPath = "://resources/dadesBCN/dadesBCNOneValue";
        visPath = "://resources/dadesBCN/setupDataBCNOneValue";
        break;
    }
    case Data2Values:{
        dataPath = "://resources/dadesBCN/data2values";
        visPath = "://resources/dadesBCN/setupData2Values";
        break;
    }
    case DataSph:{
        dataPath = "://resources/dadesBCN/dadesBCNOneValueSph";
        visPath = "://resources/dadesBCN/setupDataBCNOneValueSph";
        break;
    }
    case Bcn:{
        dataPath = "://resources/dadesBCN/dadesBCNTexture";
        visPath = "://resources/dadesBCN/setupDataBCN";
        break;
    }
    case Europa:{
        dataPath = "://resources/dadesEuropa/dadesEuropaTextured";
        visPath = "://resources/dadesEuropa/setupDataEuropa";
        break;
    }
    case Transparencies:{
        dataPath = "://resources/transparencies";
        visPath = "://resources/setupTransparencies";
        break;
    }
    case LlumCon:{
        dataPath = "://resources/coneLight";
        visPath = "://resources/setupConeLight";
        break;
    }
    case Penombres:{ //Amb llum de placa en lloc de spot o dot.
        dataPath = "://resources/penombres";
        visPath = "://resources/setupPenombres";
        break;
    }
    case AmbientOcclusion:{ //Només hi ha llum ambient en el resultat.
        dataPath = "://resources/ambientOcclusion";
        visPath = "://resources/setupAmbientOcclusion";
        break;
    }
    case DefocusBlur:{ //Enfocar només el punt de lookAt.
        dataPath = "://resources/defocusBlur";
        visPath = "://resources/setupDefocusBlur";
        break;
    }
    case Polynomial:{
        dataPath = "://resources/tests";
        visPath = "://resources/setupPoly";
        break;

    }
      
    case Earth:{
        dataPath = "://resources/earth/earth";
        visPath = "://resources/earth/setupEarth";
        break;
    }
    case Stars:{
        dataPath = "://resources/earth/stars";
        visPath = "://resources/earth/setupStars";
        break;
    }
    case EarthWithStars:{
        dataPath = "://resources/earth/earthStars";
        visPath = "://resources/earth/setupEarthStars";
        break;
    }
    case EarthWithStarsAndMoon:{
        dataPath = "://resources/earth/earthStarsMoon";
        visPath = "://resources/earth/setupEarthStarsMoon";
        break;
    }
    case TransparentSpheres:{
        dataPath = "://resources/transparentSpheres";
        visPath = "://resources/setupTests";
        break;
    }
    default:{}
    }
    auto controller = make_shared<Controller>(Serializable::Json, dataPath,visPath);
    controller->start(argc, argv);

    return(0);
}
