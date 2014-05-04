//
//  main.cpp
//  3DMathBenchmark
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>


#define GLM_FORCE_INLINE

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#ifdef GLM_PRECISION_HIGHP_FLOAT
typedef double floatType;
typedef glm::dmat3 m3;
typedef glm::dmat4 m4;
typedef glm::dvec3 v3;
typedef glm::dvec4 v4;
#else
typedef float floatType;
typedef glm::mat3 m3;
typedef glm::mat4 m4;
typedef glm::vec3 v3;
typedef glm::vec4 v4;
#endif
#include "Timer.h"


using namespace glm;

int runCount = 10;
long internalRunCount = 2000000;
Timer t;

float rnd(float max){
    return rand()*max/RAND_MAX;
}



m4 createRandomMatrix(){
    return m4(
                     rnd(100),rnd(100),rnd(100),rnd(100),
                     rnd(100),rnd(100),rnd(100),rnd(100),
                     rnd(100),rnd(100),rnd(100),rnd(100),
                     rnd(100),rnd(100),rnd(100),rnd(100)
                     );
}

double testMultiplication(long count){
    m4 m1 = createRandomMatrix();
    m4 m2 = createRandomMatrix();    
    t.start();
    for (int i = 0; i < count; i++) {
        m1 = m1 * m2;
    }
    t.stop();
    return t.getElapsedTimeInMilliSec();
}

double testTranslation(long count){
    m4 m1 = createRandomMatrix();
    v3 v1 = v3(1, 2, 3);

    t.start();
    for (int i = 0; i < count; i++) {
        m1 = glm::translate(m1,v1);
    }
    t.stop();
    return t.getElapsedTimeInMilliSec();
}

double testScale(long count){
    m4 m1 = createRandomMatrix();
    v3 v1 = v3(1, 2, 3);
    
    t.start();
    for (int i = 0; i < count; i++) {
        m1 = glm::scale(m1,v1);
    }
    t.stop();
    return t.getElapsedTimeInMilliSec();
}

double testRotationArbitraryAxis(long count){
    m4 m1 = createRandomMatrix();
    v3 v1 = v3(1, 2, 3);
    floatType a = (floatType)M_PI /2;
    t.start();
    for (int i = 0; i < count; i++) {
        m1 = glm::rotate(m1,a,v1);
    }
    t.stop();
    return t.getElapsedTimeInMilliSec();
}

double testTranspose(long count){
    m4 m1 = createRandomMatrix();

    t.start();
    for (int i = 0; i < count; i++) {
        m1 = glm::transpose(m1);
    }
    t.stop();
    if (rand()==0){ // force compiler to not optimize loop away
        std::cout<<m1[rand()%3][rand()%3]<<std::endl;
    }

    return t.getElapsedTimeInMilliSec();
}

double testInverse(long count){
    m4 m1 = createRandomMatrix();
    
    t.start();
    for (int i = 0; i < count; i++) {
        m1 = glm::inverse(m1);
    }
    t.stop();
    if (rand()==0){ // force compiler to not optimize loop away
        std::cout<<m1[rand()%3][rand()%3]<<std::endl;
    }

    return t.getElapsedTimeInMilliSec();
}

double testInverseMat3(long count){
    m4 m1 = createRandomMatrix();
    m3 m2;
    m2 = m3(m1); // note that this benchmark is actually doing less than 
    t.start();
    for (int i = 0; i < count; i++) {
        m2 = glm::inverse(m2);
    }
    t.stop();
    if (rand()==0){ // force compiler to not optimize loop away
        std::cout<<m2[rand()%3][rand()%3]<<std::endl;
    }
    return t.getElapsedTimeInMilliSec();
}

double testVectorTransformation(long count){
    m4 m1 = createRandomMatrix();
    v4 v1 = v4(1,2,3,1);
    t.start();
    for (int i = 0; i < count; i++) {
        v1 = m1*v1;
    }
    t.stop();
    if (rand()==0){ // force compiler to keep input
        std::cout<<v1[rand()%4]<<std::endl;
    }
    return t.getElapsedTimeInMilliSec();
}


void runTestCase(double(*f)(long), char* name){
    double totalTime = 0;
    double minTime = 0;
    double maxTime = 0;
    
    for(int i = 0; i < runCount; ++i) {
        double time = f(internalRunCount);
        if(i == 0) {
            minTime = time;
            maxTime = time;
        } else {
            if(minTime > time) { minTime = time; }
            if(maxTime < time) { maxTime = time; }
        }
        totalTime += time;
    }
    
    double avg = totalTime / runCount;
    
    std::cout<<name<<" - Avg: "<<avg<<"ms, Min: "<<minTime<<"ms, Max: "<<maxTime<<"ms"<<std::endl;
}


int main (int argc, const char * argv[])
{
	if (argc>=2){
		internalRunCount = atoi(argv[1]);
		std::cout<<"InternalRunCount: "<<internalRunCount<<std::endl;
	}
	if (argc>=3){
		runCount = atoi(argv[2]);
		std::cout<<"Run count: "<<runCount<<std::endl;
	}

    /* initialize random seed: */
    srand ( time(NULL) );
        
    int matrixBytes = sizeof(createRandomMatrix());
    
    std::cout<<"Using double precision "<<((matrixBytes>64)?"true":"false")<<std::endl;
    runTestCase(&testMultiplication,"testMultiplication");
    runTestCase(&testTranslation,"testTranslation");
    runTestCase(&testScale,"testScale");
    runTestCase(&testRotationArbitraryAxis,"testRotationArbitraryAxis");
    runTestCase(&testTranspose,"testTranspose");
    runTestCase(&testInverse,"testInverse");    
    runTestCase(&testInverseMat3,"testInverseMat3");    
    runTestCase(&testVectorTransformation,"testVectorTransformation");        

    
    return 0;
}

