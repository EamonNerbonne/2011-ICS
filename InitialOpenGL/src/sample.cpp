#include "RotationModel.h"
#include "EulerRotation.h"
#include "QuaternionRotation.h"
#include "InitOpenGL.h"
#include "BookRenderer.h"

using namespace Eigen;
using namespace std;

DisplayManager * RendererFactory() {
	auto state_tracker = shared_ptr<RotationModel>(
			new EulerRotation(
						Vector3d(1.878, 2.9085, 0.5),
						Vector3d(1.0,0.0,0.0),
						0.000001
			)
	);
	return new BookRenderer(state_tracker);

}

int main(int argc, char** argv) {
	SetupMainLoop(RendererFactory, argc, argv);
}

