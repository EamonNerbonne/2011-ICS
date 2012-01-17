#include "RotationModel.h"
#include "EulerRotation.h"
#include "QuaternionRotation.h"
#include "ParticleBasedRotation.h"
#include "InitOpenGL.h"
#include "BookRenderer.h"

using namespace Eigen;
using namespace std;

DisplayManager * RendererFactory() {
	auto state_tracker = shared_ptr<RotationModel>(
			new ParticleBasedRotation(
						Vector3d(1.878, 2.9085, 0.5),
						0.00001)
	);
	state_tracker->resetWithAngularMomentum(Vector3d(1.0, 0.0, 0.0));
	return new BookRenderer(state_tracker);

}

int main(int argc, char** argv) {
	SetupMainLoop(RendererFactory, argc, argv);
}

